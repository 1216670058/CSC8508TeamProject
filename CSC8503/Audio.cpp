#include "Audio.h"

using namespace std;

Audio::Audio(GameWorld* world)
{
    std::cout << std::endl << "--------Initialising Audio--------" << std::endl;

    this->world = world;
    // start the sound engine with default parameters
    soundEngine = createIrrKlangDevice();
    if (!soundEngine)
    {
        printf("Could not startup soundEngine\n");
    }
    menubgmsource = soundEngine->addSoundSourceFromFile((Assets::SOUNDSDIR + "getout.ogg").c_str());
}

Audio::~Audio()
{
    soundEngine->drop();
}

void Audio::Update()
{
    //Vector3 cp = world->GetMainCamera().GetPosition();
    //vec3df position(cp.x, cp.y, cp.z);  // position of the listener
    //Vector3 forward = Matrix4::Rotation(world->GetMainCamera().GetYaw(),
    //    Vector3(0, 1, 0)) * Matrix4::Rotation(world->GetMainCamera().GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1);
    //vec3df lookDirection(-forward.x, forward.y, -forward.z);  // the direction the listener looks into

    switch (world->GetGameState())
    {
    case GameState::LOADING:
        if (menubgm == nullptr) {
            menubgm = soundEngine->play2D(menubgmsource, true, false, true, false);
            menubgm->setVolume(0.05f);
        }
        break;
    case GameState::PLAYING:
        if (menubgm != nullptr) {
            soundEngine->stopAllSoundsOfSoundSource(menubgmsource);
            soundEngine->removeSoundSource(menubgmsource);
        }
        //bgm = soundEngine->play3D(bgmsource, vec3df(1, 0, 1), true, false, true);
        //soundEngine->setListenerPosition(position, lookDirection);
        break;
    default:
        break;
    }

}
