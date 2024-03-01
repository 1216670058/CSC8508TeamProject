#include "NetworkObject.h"
#include "RenderObject.h"
#include "AnimationObject.h"
#include "./enet/enet.h"
using namespace NCL;
using namespace CSC8503;

NetworkObject::NetworkObject(GameObject& o, int id) : object(o)	{
	deltaErrors = 0;
	fullErrors  = 0;
	networkID   = id;
}

NetworkObject::~NetworkObject()	{
}

bool NetworkObject::ReadPacket(GamePacket &p) {
    if (p.type == Delta_State) {
        return ReadDeltaPacket((DeltaPacket &)p);
    }
    if (p.type == Full_State) {
        return ReadFullPacket((FullPacket &)p);
    }
    return false; // this isn’t a packet we care about!
}

bool NetworkObject::ReadClientPacket(ClientPacket& p) {
    object.SetButton(0, p.buttonstates[0]);
    object.SetButton(1, p.buttonstates[1]);
    object.SetButton(2, p.buttonstates[2]);
    object.SetButton(3, p.buttonstates[3]);
    return true;
}

bool NetworkObject::WritePacket(GamePacket** p, bool deltaFrame, int stateID) {
	if (deltaFrame) {
		if (!WriteDeltaPacket(p, stateID)) {
			return WriteFullPacket(p);
		}
	}
	return WriteFullPacket(p);
}
//Client objects recieve these packets
bool NetworkObject::ReadDeltaPacket(DeltaPacket &p) {
    if (networkID <= 4) {
        DeltaPlayerPacket* dp = (DeltaPlayerPacket*)(&p);
        if (dp->fullID != lastFullPlayerState.stateID) {
            return false; // can’t delta this frame
        }
        UpdateStateHistory(dp->fullID);
        if (dp->objectID == object.GetNetworkObject()->GetNetworkID()) {
            Vector3 fullPos = lastFullPlayerState.position;
            Quaternion fullOrientation = lastFullPlayerState.orientation;
            int fullCurrentFrame = lastFullPlayerState.currentFrame;

            fullPos.x += dp->pos[0];
            fullPos.y += dp->pos[1];
            fullPos.z += dp->pos[2];

            fullOrientation.x += ((float)dp->orientation[0]) / 127.0f;
            fullOrientation.y += ((float)dp->orientation[1]) / 127.0f;
            fullOrientation.z += ((float)dp->orientation[2]) / 127.0f;
            fullOrientation.w += ((float)dp->orientation[3]) / 127.0f;

            fullCurrentFrame += (int)(dp->currentFrame);

            object.GetTransform().SetPosition(fullPos);
            object.GetTransform().SetOrientation(fullOrientation);
            object.GetRenderObject()->GetAnimationObject()->SetCurrentFrame(fullCurrentFrame);
        }
        return true;
    }
    else {
        if (p.fullID != lastFullState.stateID) {
            return false; // can’t delta this frame
        }
        UpdateStateHistory(p.fullID);
        if (p.objectID == object.GetNetworkObject()->GetNetworkID()) {
            Vector3 fullPos = lastFullState.position;
            Quaternion fullOrientation = lastFullState.orientation;

            fullPos.x += p.pos[0];
            fullPos.y += p.pos[1];
            fullPos.z += p.pos[2];

            fullOrientation.x += ((float)p.orientation[0]) / 127.0f;
            fullOrientation.y += ((float)p.orientation[1]) / 127.0f;
            fullOrientation.z += ((float)p.orientation[2]) / 127.0f;
            fullOrientation.w += ((float)p.orientation[3]) / 127.0f;

            object.GetTransform().SetPosition(fullPos);
            object.GetTransform().SetOrientation(fullOrientation);
        }
        return true;
    }
}


bool NetworkObject::ReadFullPacket(FullPacket& p) {
    if (networkID <= 4) {
        FullPlayerPacket* fp = (FullPlayerPacket*)(&p);
        if (fp->fullPlayerState.stateID < lastFullPlayerState.stateID) {
            return false; // received an 'old' packet, ignore!
        }
        lastFullPlayerState = fp->fullPlayerState;
        if (fp->objectID == object.GetNetworkObject()->GetNetworkID()) {
            object.GetTransform().SetPosition(lastFullPlayerState.position);
            object.GetTransform().SetOrientation(lastFullPlayerState.orientation);
            object.GetRenderObject()->GetAnimationObject()->SetCurrentFrame(lastFullPlayerState.currentFrame);
        }
        if (networkID == 1)
            std::cout << "Player1Recieved: " << object.GetTransform().GetPosition().x << " " <<
            object.GetTransform().GetPosition().y << " " <<
            object.GetTransform().GetPosition().z << " " << std::endl;
        stateHistory.emplace_back(lastFullPlayerState);

        return true;
    }
    else {
        if (p.fullState.stateID < lastFullState.stateID) {
            return false; // received an 'old' packet, ignore!
        }
        lastFullState = p.fullState;
        if (p.objectID == object.GetNetworkObject()->GetNetworkID()) {
            object.GetTransform().SetPosition(lastFullState.position);
            object.GetTransform().SetOrientation(lastFullState.orientation);
        }

        stateHistory.emplace_back(lastFullState);

        return true;
    }
}
bool NetworkObject::WriteDeltaPacket(GamePacket** p, int stateID) {
    if (networkID <= 4) {
        DeltaPlayerPacket* dpp = new DeltaPlayerPacket();
        PlayerState playerState;
        if (!GetNetworkState(stateID, playerState)) {
            return false; // can’t delta!
        }

        dpp->fullID = stateID;
        dpp->objectID = networkID;

        Vector3 currentPos = object.GetTransform().GetPosition();
        Quaternion currentOrientation = object.GetTransform().GetOrientation();
        int currentFrame = object.GetRenderObject()->GetAnimationObject()->GetCurrentFrame();

        currentPos -= playerState.position;
        currentOrientation -= playerState.orientation;
        currentFrame -= playerState.currentFrame;
        

        dpp->pos[0] = (char)currentPos.x;
        dpp->pos[1] = (char)currentPos.y;
        dpp->pos[2] = (char)currentPos.z;

        dpp->orientation[0] = (char)(currentOrientation.x * 127.0f);
        dpp->orientation[1] = (char)(currentOrientation.y * 127.0f);
        dpp->orientation[2] = (char)(currentOrientation.z * 127.0f);
        dpp->orientation[3] = (char)(currentOrientation.w * 127.0f);

        dpp->currentFrame = (char)(currentFrame);

        *p = dpp;
        return true;
    }
    else {
        DeltaPacket* dp = new DeltaPacket();
        NetworkState state;
        if (!GetNetworkState(stateID, state)) {
            return false; // can’t delta!
        }

        dp->fullID = stateID;
        dp->objectID = networkID;

        Vector3 currentPos = object.GetTransform().GetPosition();
        Quaternion currentOrientation = object.GetTransform().GetOrientation();

        currentPos -= state.position;
        currentOrientation -= state.orientation;

        dp->pos[0] = (char)currentPos.x;
        dp->pos[1] = (char)currentPos.y;
        dp->pos[2] = (char)currentPos.z;

        dp->orientation[0] = (char)(currentOrientation.x * 127.0f);
        dp->orientation[1] = (char)(currentOrientation.y * 127.0f);
        dp->orientation[2] = (char)(currentOrientation.z * 127.0f);
        dp->orientation[3] = (char)(currentOrientation.w * 127.0f);

        *p = dp;
        return true;
    }
}

bool NetworkObject::WriteFullPacket(GamePacket** p) {
    if (networkID <= 4) {
        FullPlayerPacket* fpp = new FullPlayerPacket();
        fpp->objectID = networkID;
        fpp->fullPlayerState.position = object.GetTransform().GetPosition();
        fpp->fullPlayerState.orientation = object.GetTransform().GetOrientation();
        fpp->fullPlayerState.currentFrame = object.GetRenderObject()->GetAnimationObject()->GetCurrentFrame();
        fpp->fullPlayerState.stateID = lastFullPlayerState.stateID++;

        *p = fpp;
        if (networkID == 1)
            std::cout << "Player1Send: " << object.GetTransform().GetPosition().x << " " <<
            object.GetTransform().GetPosition().y << " " <<
            object.GetTransform().GetPosition().z << " " << std::endl;
        return true;
    }
    else {
        FullPacket* fp = new FullPacket();
        fp->objectID = networkID;
        fp->fullState.position = object.GetTransform().GetPosition();
        fp->fullState.orientation = object.GetTransform().GetOrientation();
        fp->fullState.stateID = lastFullState.stateID++;

        *p = fp;
        return true;
    }
}

void NetworkObject::UpdateStateHistory(int minID) {
    for (auto i = stateHistory.begin(); i < stateHistory.end();) {
        if ((*i).stateID < minID) {
            i = stateHistory.erase(i);
        }
        else {
            ++i;
        }
    }
}

NetworkState& NetworkObject::GetLatestNetworkState() {
    return lastFullState;
}

bool NetworkObject::GetNetworkState(int stateID, NetworkState& state) {
    for (auto i = stateHistory.begin(); i < stateHistory.end(); ++i) {
        if ((*i).stateID == stateID) {
            state = (*i);
            return true;
        }
    }
    return false;
}