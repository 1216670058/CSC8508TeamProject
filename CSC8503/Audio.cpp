#include "Audio.h"
#include "TutorialGame.h"

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
    menubgmsource = soundEngine->addSoundSourceFromFile((Assets::SOUNDSDIR + "Menu.mp3").c_str());
    playingbgmsource = soundEngine->addSoundSourceFromFile((Assets::SOUNDSDIR + "PlayingBGM.mp3").c_str());
    trainsource = soundEngine->addSoundSourceFromFile((Assets::SOUNDSDIR + "train.mp3").c_str());

}

Audio::~Audio()
{
    soundEngine->drop();
}

void Audio::Update()
{
    Vector3 lp;
    Vector3 tp;
    vec3df lposition;
    vec3df tposition;
    Vector3 forward;
    vec3df lookDirection;
    switch (world->GetGameState())
    {
    case GameState::LOADING:
    case GameState::MENU:
        soundEngine->setAllSoundsPaused(false);
        if (menubgm == nullptr) {
            soundEngine->stopAllSoundsOfSoundSource(playingbgmsource);
            soundEngine->stopAllSoundsOfSoundSource(trainsource);
            if (playingbgm != nullptr)
            {
                playingbgm->drop();
                playingbgm = nullptr;
                trainbgm->drop();
                trainbgm = nullptr;
            }
            menubgm = soundEngine->play2D(menubgmsource, true, false, true, false);
            menubgm->setVolume(0.075f);
        }
        break;
    case GameState::PLAYING:
        lp = TutorialGame::GetGame()->GetPlayer()->GetTransform().GetPosition();
        lposition = vec3df(lp.x, lp.y, lp.z);  // position of the listener
        forward = TutorialGame::GetGame()->GetPlayer()->GetFace();
        //Vector3 forward = Matrix4::Rotation(world->GetMainCamera().GetYaw(),
        //    Vector3(0, 1, 0)) * Matrix4::Rotation(world->GetMainCamera().GetPitch(), Vector3(1, 0, 0)) * Vector3(0, 0, -1);
        lookDirection = vec3df(-forward.x, forward.y, -forward.z);  // the direction the listener looks into
        soundEngine->setAllSoundsPaused(false);
        if (menubgm != nullptr) {
            soundEngine->stopAllSoundsOfSoundSource(menubgmsource);
            menubgm->drop();
            menubgm = nullptr;
            if (playingbgm == nullptr)
            {
                playingbgm = soundEngine->play2D(playingbgmsource, true, false, true, false);
                playingbgm->setVolume(0.05f);
                trainbgm = soundEngine->play3D(trainsource, vec3df(1, 0, 1), true, false, true);
                trainbgm->setVolume(0.5f);
            }
        }
        tp = TutorialGame::GetGame()->GetTrain()->GetTransform().GetPosition();
        tposition = vec3df(tp.x, tp.y, tp.z);
        trainbgm->setPosition(tposition);
        soundEngine->setListenerPosition(lposition, lookDirection);
        break;
    case GameState::PAUSED:
        soundEngine->setAllSoundsPaused(true);
        break;
    default:
        break;
    }

}

void Audio::playbutton()
{
    button = soundEngine->play2D((Assets::SOUNDSDIR + "Button.mp3").c_str(), false, true, true);
    button->setVolume(0.25);
    button->setIsPaused(false);
}
