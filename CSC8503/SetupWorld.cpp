#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "StateGameObject.h"

void TutorialGame::InitialiseAssets() {
    std::cout << std::endl << "--------Initialising Assets--------" << std::endl;

    std::cout << std::endl << "--------Loading Meshes--------" << std::endl;
    cubeMesh = renderer->LoadMesh("cube.msh");
    sphereMesh = renderer->LoadMesh("sphere.msh");
    charMesh = renderer->LoadMesh("goat.msh");
    enemyMesh = renderer->LoadMesh("Keeper.msh");
    bonusMesh = renderer->LoadMesh("apple.msh");
    capsuleMesh = renderer->LoadMesh("capsule.msh");
    woodTex = renderer->LoadTexture("wood.png");
    floorBumpTex = renderer->LoadTexture("grassbump.png");
    trainMesh = renderer->LoadOBJMesh("Train.obj");
    carriageMesh = renderer->LoadMesh("Carriage.msh");
    treeMesh = renderer->LoadMesh("Tree.msh");
    rockMesh = renderer->LoadMesh("Rock.msh");
    creeperMesh = renderer->LoadOBJMesh("Creeper.obj");
    pickaxeMesh = renderer->LoadMesh("Pickaxe.msh");
    axeMesh = renderer->LoadMesh("Axe.msh");
    bucketMesh = renderer->LoadMesh("Bucket_Empty.msh");
    //plankMesh = renderer->LoadMesh("Plank.msh");
    //stoneMesh = renderer->LoadMesh("Stone.msh");
    //railMesh = renderer->LoadMesh("Rail.msh");
    maleMesh = renderer->LoadMesh("Male_Guard.msh");
    //femaleMesh = renderer->LoadMesh("Female_Guard.msh");
    //assassinMesh = renderer->LoadMesh("Assassin.msh");
    //girlMesh = renderer->LoadMesh("Girl.msh");
    //smurfMesh = renderer->LoadMesh("Smurf.msh");
    mooseMesh = renderer->LoadMesh("Moose.msh");
    robotMesh = renderer->LoadMesh("Robot.msh");
    droneMesh = renderer->LoadMesh("Drone.msh");

    meshes.push_back(maleMesh);
    //meshes.push_back(femaleMesh);
    //meshes.push_back(assassinMesh);
    //meshes.push_back(girlMesh);
    //meshes.push_back(smurfMesh);

    std::cout << std::endl << "--------Loading Textures--------" << std::endl;
    basicTex = renderer->LoadTexture("checkerboard.png");
    woodTex = renderer->LoadTexture("wood.png");
    trainTex = renderer->LoadTexture("Train.jpg");
    carriageTex = renderer->LoadTexture("CartEmpty_T.png");
    treeTex = renderer->LoadTexture("Tree.png");
    rockTex = renderer->LoadTexture("Rock.png");
    lightTex = renderer->LoadTexture("redstone_lamp_on.png");
    pickaxeTex = renderer->LoadTexture("lowpoly_pickaxe_BaseColor.png");
    axeTex = renderer->LoadTexture("Axe_albedo.jpg");
    bucketTex = renderer->LoadTexture("lambert2_Base_Color.png");
    //plankTex = renderer->LoadTexture("Planks_Diff.png");
    //stoneTex = renderer->LoadTexture("Stone.png");
    //railTex = renderer->LoadTexture("Rail.png");

    floorBumpTex = renderer->LoadTexture("grassbump.png");;
    rockBumpTex = renderer->LoadTexture("Rock_n.png");
    lightBumpTex = renderer->LoadTexture("redstone_lamp_on_n.png");
    pickaxeBumpTex = renderer->LoadTexture("lowpoly_pickaxe_Normal.png");
    axeBumpTex = renderer->LoadTexture("Axe_normal.png");
    bucketBumpTex = renderer->LoadTexture("lambert2_Normal_OpenGL.png");
    //plankBumpTex = renderer->LoadTexture("Planks_Norm.png");
    //stoneBumpTex = renderer->LoadTexture("Stone_n.png");
    //railBumpTex = renderer->LoadTexture("Rail_n.png");

    lightSpecTex = renderer->LoadTexture("redstone_lamp_on_s.png");

    InitMaterials();
    InitAnimations();

    std::cout << std::endl << "--------Loading Shader Groups--------" << std::endl;
    basicDayShader = renderer->LoadShader("PerPixel.vert", "PerPixelScene.frag");
    bumpDayShader = renderer->LoadShader("Bump.vert", "BumpScene.frag");
    specDayShader = renderer->LoadShader("Bump.vert", "SpecScene.frag");
    skinningPerPixelDayShader = renderer->LoadShader("SkinningPerPixel.vert", "SkinningPerPixelScene.frag");
    skinningBumpDayShader = renderer->LoadShader("SkinningBump.vert", "SkinningBumpScene.frag");
    skinningBumpDayShader2 = renderer->LoadShader("SkinningBump.vert", "BumpScene.frag");

    basicNightShader = renderer->LoadShader("PerPixel.vert", "PerPixelBuffer.frag");
    bumpNightShader = renderer->LoadShader("Bump.vert", "BumpBuffer.frag");
    specNightShader = renderer->LoadShader("Bump.vert", "SpecBuffer.frag");
    skinningPerPixelNightShader = renderer->LoadShader("SkinningPerPixel.vert", "SkinningPerPixelBuffer.frag");
    skinningBumpNightShader = renderer->LoadShader("SkinningBump.vert", "SkinningBumpBuffer.frag");
    skinningBumpNightShader2 = renderer->LoadShader("SkinningBump.vert", "BumpBuffer.frag");

    basicShader = new ShaderGroup(basicDayShader, basicNightShader);
    bumpShader = new ShaderGroup(bumpDayShader, bumpNightShader);
    specShader = new ShaderGroup(specDayShader, specNightShader);
    skinningPerPixelShader = new ShaderGroup(skinningPerPixelDayShader, skinningPerPixelNightShader);
    skinningBumpShader = new ShaderGroup(skinningBumpDayShader, skinningBumpNightShader);
    skinningBumpShader2 = new ShaderGroup(skinningBumpDayShader2, skinningBumpNightShader2);

    shaders.push_back(skinningBumpShader);
    shaders.push_back(skinningBumpShader);
    shaders.push_back(skinningPerPixelShader);
    shaders.push_back(skinningBumpShader);
    shaders.push_back(skinningBumpShader);

    InitCamera();
    InitWorld();
}

void TutorialGame::InitMaterials() {
    std::cout << std::endl << "--------Loading Materials--------" << std::endl;
    maleMaterial = new MeshMaterial("Male_Guard.mat");
    for (int i = 0; i < maleMesh->GetSubMeshCount(); ++i) {
        const MeshMaterialEntry* matEntry =
            maleMaterial->GetMaterialForLayer(i);

        const string* filename = nullptr;
        matEntry->GetEntry("Diffuse", &filename);
        string path = Assets::TEXTUREDIR + *filename;
        GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        maleTextures.emplace_back(texID);

        matEntry->GetEntry("Bump", &filename);
        string path2 = Assets::TEXTUREDIR + *filename;
        GLuint texID2 = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        maleBumpTextures.emplace_back(texID2);
    }

    //femaleMaterial = new MeshMaterial("Female_Guard.mat");
    //for (int i = 0; i < femaleMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        femaleMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    femaleTextures.emplace_back(texID);
    //
    //    matEntry->GetEntry("Bump", &filename);
    //    string path2 = Assets::TEXTUREDIR + *filename;
    //    GLuint texID2 = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    femaleBumpTextures.emplace_back(texID2);
    //}
    //
    //assassinMaterial = new MeshMaterial("Assassin.mat");
    //for (int i = 0; i < assassinMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        assassinMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    assassinTextures.emplace_back(texID);
    //}
    //vector<GLuint> assassinBumpTextures;
    //
    //girlMaterial = new MeshMaterial("Girl.mat");
    //for (int i = 0; i < girlMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        girlMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    girlTextures.emplace_back(texID);
    //
    //    matEntry->GetEntry("Bump", &filename);
    //    string path2 = Assets::TEXTUREDIR + *filename;
    //    GLuint texID2 = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    girlBumpTextures.emplace_back(texID2);
    //}
    //
    //smurfMaterial = new MeshMaterial("Smurf.mat");
    //for (int i = 0; i < smurfMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        smurfMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    smurfTextures.emplace_back(texID);
    //
    //    matEntry->GetEntry("Bump", &filename);
    //    string path2 = Assets::TEXTUREDIR + *filename;
    //    GLuint texID2 = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    smurfBumpTextures.emplace_back(texID2);
    //}

    //mooseMaterial = new MeshMaterial("Moose.mat");
    //for (int i = 0; i < mooseMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        mooseMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    mooseTextures.emplace_back(texID);
    //}
    //
    //robotMaterial = new MeshMaterial("Robot.mat");
    //for (int i = 0; i < robotMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        robotMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    robotTextures.emplace_back(texID);
    //
    //    matEntry->GetEntry("Bump", &filename);
    //    string path2 = Assets::TEXTUREDIR + *filename;
    //    GLuint texID2 = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    robotBumpTextures.emplace_back(texID2);
    //}
    //
    //droneMaterial = new MeshMaterial("Drone.mat");
    //for (int i = 0; i < droneMesh->GetSubMeshCount(); ++i) {
    //    const MeshMaterialEntry* matEntry =
    //        droneMaterial->GetMaterialForLayer(i);
    //
    //    const string* filename = nullptr;
    //    matEntry->GetEntry("Diffuse", &filename);
    //    string path = Assets::TEXTUREDIR + *filename;
    //    GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
    //        SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    droneTextures.emplace_back(texID);
    //}

    textures.push_back(maleTextures);
    //textures.push_back(femaleTextures);
    //textures.push_back(assassinTextures);
    //textures.push_back(girlTextures);
    //textures.push_back(smurfTextures);

    bumpTextures.push_back(maleBumpTextures);
    //bumpTextures.push_back(femaleBumpTextures);
    //bumpTextures.push_back(assassinBumpTextures);
    //bumpTextures.push_back(girlBumpTextures);
    //bumpTextures.push_back(smurfBumpTextures);
}

void TutorialGame::InitAnimations() {
    std::cout << std::endl << "--------Loading Animations--------" << std::endl;
    maleAnimation = new AnimationObject();
    maleAnimation->SetAnim1(new MeshAnimation("Idle1.anm"));
    maleAnimation->SetAnim2(new MeshAnimation("StepForward1.anm"));
    maleAnimation->SetAnim3(new MeshAnimation("StepLeft1.anm"));
    maleAnimation->SetAnim4(new MeshAnimation("StepRight1.anm"));
    maleAnimation->SetAnim5(new MeshAnimation("StepBack1.anm"));
    maleAnimation->SetAnim6(new MeshAnimation("Angry1.anm"));
    maleAnimation->SetAnim7(new MeshAnimation("Happy1.anm"));
    maleAnimation->SetAnim8(new MeshAnimation("Cheer1.anm"));
    maleAnimation->SetActiveAnim(maleAnimation->GetAnim1());

    //femaleAnimation = new AnimationObject();
    //femaleAnimation->SetAnim1(new MeshAnimation("Idle2.anm"));
    //femaleAnimation->SetAnim2(new MeshAnimation("StepForward2.anm"));
    //femaleAnimation->SetAnim3(new MeshAnimation("StepLeft2.anm"));
    //femaleAnimation->SetAnim4(new MeshAnimation("StepRight2.anm"));
    //femaleAnimation->SetAnim5(new MeshAnimation("StepBack2.anm"));
    //femaleAnimation->SetAnim6(new MeshAnimation("Angry2.anm"));
    //femaleAnimation->SetAnim7(new MeshAnimation("Happy2.anm"));
    //femaleAnimation->SetAnim8(new MeshAnimation("Cheer2.anm"));
    //femaleAnimation->SetActiveAnim(femaleAnimation->GetAnim1());
    //
    //assassinAnimation = new AnimationObject();
    //assassinAnimation->SetAnim1(new MeshAnimation("Assassin.anm"));
    //assassinAnimation->SetAnim2(new MeshAnimation("Assassin.anm"));
    //assassinAnimation->SetAnim3(new MeshAnimation("Assassin.anm"));
    //assassinAnimation->SetAnim4(new MeshAnimation("Assassin.anm"));
    //assassinAnimation->SetAnim5(new MeshAnimation("Assassin.anm"));
    //assassinAnimation->SetActiveAnim(assassinAnimation->GetAnim1());
    //assassinAnimation->SetIdle(false);
    //
    //girlAnimation = new AnimationObject();
    //girlAnimation->SetAnim1(new MeshAnimation("Girl.anm"));
    //girlAnimation->SetAnim2(new MeshAnimation("Girl.anm"));
    //girlAnimation->SetAnim3(new MeshAnimation("Girl.anm"));
    //girlAnimation->SetAnim4(new MeshAnimation("Girl.anm"));
    //girlAnimation->SetAnim5(new MeshAnimation("Girl.anm"));
    //girlAnimation->SetActiveAnim(girlAnimation->GetAnim1());
    //girlAnimation->SetIdle(false);
    //
    //smurfAnimation = new AnimationObject();
    //smurfAnimation->SetAnim1(new MeshAnimation("Smurf.anm"));
    //smurfAnimation->SetAnim2(new MeshAnimation("Smurf.anm"));
    //smurfAnimation->SetAnim3(new MeshAnimation("Smurf.anm"));
    //smurfAnimation->SetAnim4(new MeshAnimation("Smurf.anm"));
    //smurfAnimation->SetAnim5(new MeshAnimation("Smurf.anm"));
    //smurfAnimation->SetActiveAnim(smurfAnimation->GetAnim1());
    //smurfAnimation->SetIdle(false);

    //mooseAnimation = new AnimationObject();
    //mooseAnimation->SetAnim1(new MeshAnimation("Moose.anm"));
    //mooseAnimation->SetAnim2(new MeshAnimation("Moose.anm"));
    //mooseAnimation->SetAnim3(new MeshAnimation("Moose.anm"));
    //mooseAnimation->SetAnim4(new MeshAnimation("Moose.anm"));
    //mooseAnimation->SetAnim5(new MeshAnimation("Moose.anm"));
    //mooseAnimation->SetActiveAnim(mooseAnimation->GetAnim1());
    //
    //robotAnimation = new AnimationObject();
    //robotAnimation->SetAnim1(new MeshAnimation("Robot.anm"));
    //robotAnimation->SetAnim2(new MeshAnimation("Robot.anm"));
    //robotAnimation->SetAnim3(new MeshAnimation("Robot.anm"));
    //robotAnimation->SetAnim4(new MeshAnimation("Robot.anm"));
    //robotAnimation->SetAnim5(new MeshAnimation("Robot.anm"));
    //robotAnimation->SetActiveAnim(robotAnimation->GetAnim1());
    //
    //droneAnimation = new AnimationObject();
    //droneAnimation->SetAnim1(new MeshAnimation("Drone.anm"));
    //droneAnimation->SetAnim2(new MeshAnimation("Drone.anm"));
    //droneAnimation->SetAnim3(new MeshAnimation("Drone.anm"));
    //droneAnimation->SetAnim4(new MeshAnimation("Drone.anm"));
    //droneAnimation->SetAnim5(new MeshAnimation("Drone.anm"));
    //droneAnimation->SetActiveAnim(droneAnimation->GetAnim1());

    animations.push_back(maleAnimation);
    //animations.push_back(femaleAnimation);
    //animations.push_back(assassinAnimation);
    //animations.push_back(girlAnimation);
    //animations.push_back(smurfAnimation);
}

StateGameObject* TutorialGame::AddStateObjectToWorld(const Vector3& position) {
    StateGameObject* cube = new StateGameObject();

    Vector3 dimensions = Vector3(1, 1, 1);

    AABBVolume* volume = new AABBVolume(dimensions);
    cube->SetBoundingVolume((CollisionVolume*)volume);

    cube->GetTransform()
        .SetPosition(position)
        .SetScale(dimensions * 2);

    cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
    cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

    cube->GetPhysicsObject()->SetInverseMass(1.0f);
    cube->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(cube);

    return cube;
}

void TutorialGame::AddSceneToWorld()
{
    int nodeSize;
    int gridWidth;
    int gridHeight;
    std::ifstream infile(Assets::DATADIR + "TestGrid1.txt");
    infile >> nodeSize;
    infile >> gridWidth;
    infile >> gridHeight;
    navGrid = new NavigationGrid("TestGrid1.txt");

    GridNode* nodes = new GridNode[gridWidth * gridHeight];
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x)
        {
            GridNode& n = nodes[(gridWidth * y) + x];
            char type = 0;
            infile >> type;
            n.type = type;
            n.position = Vector3((float)(x * nodeSize), 7, (float)(y * nodeSize));
            if (type == 120)scene.emplace_back(AddCubeToWorld(n.position, { (float)nodeSize / 2,(float)nodeSize / 2,(float)nodeSize / 2 }, 0));

        }
    }
    return;
}

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
    GameObject* floor = new GameObject("Floor");

    Vector3 floorSize = Vector3(200, 2, 200);
    AABBVolume* volume = new AABBVolume(floorSize);
    floor->SetBoundingVolume((CollisionVolume*)volume);
    floor->GetTransform()
        .SetScale(floorSize * 2)
        .SetPosition(position);

    floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, woodTex, basicShader));
    floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

    floor->GetPhysicsObject()->SetInverseMass(0);
    floor->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(floor);

    return floor;
}

GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
    GameObject* sphere = new GameObject();

    Vector3 sphereSize = Vector3(radius, radius, radius);
    SphereVolume* volume = new SphereVolume(radius);
    sphere->SetBoundingVolume((CollisionVolume*)volume);

    sphere->GetTransform()
        .SetScale(sphereSize)
        .SetPosition(position);

    sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
    sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

    sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
    sphere->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(sphere);

    return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
    GameObject* cube = new GameObject();

    AABBVolume* volume = new AABBVolume(dimensions);
    cube->SetBoundingVolume((CollisionVolume*)volume);

    cube->GetTransform()
        .SetPosition(position)
        .SetScale(dimensions * 2);

    cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
    cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

    cube->GetPhysicsObject()->SetInverseMass(inverseMass);
    cube->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(cube);

    return cube;
}

GameObject* TutorialGame::AddPlayer0ToWorld(const Vector3& position) {
    float meshSize = 1.0f;
    float inverseMass = 0.5f;

    character = new GameObject("Player");
    SphereVolume* volume = new SphereVolume(1.0f);

    character->SetBoundingVolume((CollisionVolume*)volume);

    character->GetTransform()
        .SetScale(Vector3(meshSize, meshSize, meshSize))
        .SetPosition(position);

    character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
    character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

    character->GetPhysicsObject()->SetInverseMass(inverseMass);
    character->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(character);

    return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
    float meshSize = 3.0f;
    float inverseMass = 0.5f;

    GameObject* character = new GameObject("Enemy");

    AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
    character->SetBoundingVolume((CollisionVolume*)volume);

    character->GetTransform()
        .SetScale(Vector3(meshSize, meshSize, meshSize))
        .SetPosition(position);

    character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));// todo can change capsule
    character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

    character->GetPhysicsObject()->SetInverseMass(inverseMass);
    character->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(character);

    return character;
}

TrainObject* TutorialGame::AddTrainToWorld(const Vector3& position) {
    TrainObject* train = new TrainObject(world);

    SphereVolume* volume = new SphereVolume(0.5f);
    train->SetBoundingVolume((CollisionVolume*)volume);
    train->GetTransform()
        .SetScale(Vector3(2, 2, 2))
        .SetPosition(position);

    train->SetRenderObject(new RenderObject(&train->GetTransform(), trainMesh, nullptr, basicShader));
    train->SetPhysicsObject(new PhysicsObject(&train->GetTransform(), train->GetBoundingVolume()));

    train->GetPhysicsObject()->SetInverseMass(1.0f);
    train->GetPhysicsObject()->InitSphereInertia();
    train->UploadAssets(carriageMesh, carriageTex, basicShader);

    world->AddGameObject(train);
    return train;
}

GameObject* TutorialGame::AddTestingLightToWorld(const Vector3& position, const Vector4& colour) {
    GameObject* cube = new GameObject();

    AABBVolume* volume = new AABBVolume(Vector3(0.5, 0.5, 0.5));
    cube->SetBoundingVolume((CollisionVolume*)volume);

    cube->GetTransform()
        .SetPosition(position)
        .SetScale(Vector3(1, 1, 1));

    cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, lightTex, bumpShader));
    cube->GetRenderObject()->SetBumpTexture(lightBumpTex);
    cube->GetRenderObject()->SetSpecTexture(lightSpecTex);
    cube->GetRenderObject()->SetColour(colour);
    cube->GetRenderObject()->SetEmissive(true);

    cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

    cube->GetPhysicsObject()->SetInverseMass(1);
    cube->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(cube);

    return cube;
}

PlayerObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
    player = new PlayerObject("Player");
    AABBVolume* volume = new AABBVolume(Vector3(1.5, 1.5, 1.5));
    player->SetBoundingVolume((CollisionVolume*)volume);

    player->GetTransform()
        .SetPosition(position)
        .SetScale(Vector3(3, 3, 3));

    player->SetRenderObject(new RenderObject(&player->GetTransform(), maleMesh, nullptr, skinningBumpShader, 3));
    player->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
    player->GetRenderObject()->SetAnimationObject(maleAnimation);
    player->GetRenderObject()->SetTextures(maleTextures);
    player->GetRenderObject()->SetBumpTextures(maleBumpTextures);

    player->SetPlayerMeshes(meshes);
    player->SetPlayerTextures(textures);
    player->SetPlayerBumpTextures(bumpTextures);
    player->SetPlayerAnimations(animations);
    player->SetPlayerShaders(shaders);

    player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));
    player->GetPhysicsObject()->SetInverseMass(1);
    player->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(player);

    return player;
}

TreeObject* TutorialGame::AddTreeToWorld(const Vector3& position) {
    float meshSize = 2.0f;
    float inverseMass = 0.5f;

    TreeObject* tree = new TreeObject();
    tree->Setscale(meshSize);
    AABBVolume* volume = new AABBVolume(Vector3(0.3f, 4.0f, 0.3f) * tree->Getscale());
    tree->SetBoundingVolume((CollisionVolume*)volume);
    tree->GetTransform()
        .SetScale(Vector3(tree->Getscale(), tree->Getscale(), tree->Getscale()))
        .SetPosition(tree->FindNearestGridCenter(position));

    tree->SetRenderObject(new RenderObject(&tree->GetTransform(), treeMesh, treeTex, basicShader));// todo can change capsule
    tree->SetPhysicsObject(new PhysicsObject(&tree->GetTransform(), tree->GetBoundingVolume()));

    tree->GetPhysicsObject()->SetInverseMass(inverseMass);
    tree->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(tree);

    return tree;
}

RockObject* TutorialGame::AddRockToWorld(const Vector3& position) {
    float meshSize = 2.0f;
    float inverseMass = 0.5f;

    RockObject* rock = new RockObject();
    rock->Setscale(meshSize);
    AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * rock->Getscale());
    rock->SetBoundingVolume((CollisionVolume*)volume);
    rock->GetTransform()
        .SetScale(Vector3(rock->Getscale(), rock->Getscale(), rock->Getscale()))
        .SetPosition(rock->FindNearestGridCenter(position));

    rock->SetRenderObject(new RenderObject(&rock->GetTransform(), rockMesh, rockTex, bumpShader));// todo can change capsule
    rock->GetRenderObject()->SetBumpTexture(rockBumpTex);
    rock->SetPhysicsObject(new PhysicsObject(&rock->GetTransform(), rock->GetBoundingVolume()));

    rock->GetPhysicsObject()->SetInverseMass(inverseMass);
    rock->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(rock);

    return rock;
}

CollectableObject* TutorialGame::AddCollectableObjectToGround(int objectId)
{
    Vector3 GroundPos = object->FindNearestGridCenter(object->PlayerFront());
    object->SetActive(false);
    groundObject = new CollectableObject(world);
    AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));
    groundObject->SetBoundingVolume((CollisionVolume*)volume);
    groundObject->GetTransform().SetPosition(GroundPos);
    groundObject->GetTransform().SetScale(Vector3(1, 1, 1));

    groundObject->SetPhysicsObject(new PhysicsObject(&object->GetTransform(), object->GetBoundingVolume()));
    groundObject->GetPhysicsObject()->SetInverseMass(1.0f);
    groundObject->GetPhysicsObject()->InitCubeInertia();
    switch (objectId)
    {
    case 1:
        groundObject->SetRenderObject(new RenderObject(&object->GetTransform(), sphereMesh, nullptr, basicShader));
        break;
    case 2:

        break;
    default:

        break;
    }
    world->AddGameObject(object);

    return groundObject;
}

PickaxeObject* TutorialGame::AddPickaxeToWorld(const Vector3& position) {
    PickaxeObject* pickaxe = new PickaxeObject(world, "Pickaxe");

    AABBVolume* volume = new AABBVolume(Vector3(0.5f, 0.5f, 0.5f));
    pickaxe->SetBoundingVolume((CollisionVolume*)volume);

    pickaxe->SetPlayer(player);

    pickaxe->GetTransform()
        .SetPosition(pickaxe->FindNearestGridCenter(position))
        .SetScale(Vector3(1, 1, 1));

    pickaxe->SetRenderObject(new RenderObject(&pickaxe->GetTransform(), pickaxeMesh, pickaxeTex, bumpShader));
    pickaxe->SetPhysicsObject(new PhysicsObject(&pickaxe->GetTransform(), pickaxe->GetBoundingVolume()));
    pickaxe->GetRenderObject()->SetBumpTexture(pickaxeBumpTex);
    pickaxe->GetPhysicsObject()->SetGravity(false);
    pickaxe->GetPhysicsObject()->SetResolve(false);

    pickaxe->GetPhysicsObject()->SetInverseMass(1);
    pickaxe->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(pickaxe);

    return pickaxe;
}

AxeObject* TutorialGame::AddAxeToWorld(const Vector3& position) {
    AxeObject* axe = new AxeObject(world, "Axe");

    AABBVolume* volume = new AABBVolume(Vector3(0.1f, 0.1f, 0.1f));
    axe->SetBoundingVolume((CollisionVolume*)volume);

    axe->SetPlayer(player);

    axe->GetTransform()
        .SetPosition(axe->FindNearestGridCenter(position))
        .SetScale(Vector3(0.2f, 0.2f, 0.2f));

    axe->SetRenderObject(new RenderObject(&axe->GetTransform(), axeMesh, axeTex, bumpShader));
    axe->SetPhysicsObject(new PhysicsObject(&axe->GetTransform(), axe->GetBoundingVolume()));
    axe->GetRenderObject()->SetBumpTexture(axeBumpTex);
    axe->GetPhysicsObject()->SetGravity(false);
    axe->GetPhysicsObject()->SetResolve(false);

    axe->GetPhysicsObject()->SetInverseMass(1);
    axe->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(axe);

    return axe;
}

BucketObject* TutorialGame::AddBucketToWorld(const Vector3& position) {
    BucketObject* bucket = new BucketObject(world, "Bucket");

    AABBVolume* volume = new AABBVolume(Vector3(0.5f, 0.5f, 0.5f));
    bucket->SetBoundingVolume((CollisionVolume*)volume);

    bucket->SetPlayer(player);

    bucket->GetTransform()
        .SetPosition(bucket->FindNearestGridCenter(position))
        .SetScale(Vector3(1, 1, 1));

    bucket->SetRenderObject(new RenderObject(&bucket->GetTransform(), bucketMesh, bucketTex, bumpShader));
    bucket->GetRenderObject()->SetBumpTexture(bucketBumpTex);
    bucket->SetPhysicsObject(new PhysicsObject(&bucket->GetTransform(), bucket->GetBoundingVolume()));
    bucket->GetPhysicsObject()->SetGravity(false);
    bucket->GetPhysicsObject()->SetResolve(false);

    bucket->GetPhysicsObject()->SetInverseMass(1);
    bucket->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(bucket);

    return bucket;
}

PlankObject* TutorialGame::AddPlankToWorld(const Vector3& position) {
    PlankObject* plank = new PlankObject(world);

    AABBVolume* volume = new AABBVolume(Vector3(0.5f, 0.5f, 0.5f));
    plank->SetBoundingVolume((CollisionVolume*)volume);

    plank->SetPlayer(player);

    plank->GetTransform()
        .SetPosition(plank->FindNearestGridCenter(position))
        .SetScale(Vector3(1, 1, 1));

    plank->SetRenderObject(new RenderObject(&plank->GetTransform(), plankMesh, plankTex, bumpShader));
    plank->GetRenderObject()->SetBumpTexture(plankBumpTex);
    plank->SetPhysicsObject(new PhysicsObject(&plank->GetTransform(), plank->GetBoundingVolume()));
    plank->GetPhysicsObject()->SetGravity(false);
    plank->GetPhysicsObject()->SetResolve(false);

    plank->GetPhysicsObject()->SetInverseMass(1);
    plank->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(plank);

    return plank;
}

StoneObject* TutorialGame::AddStoneToWorld(const Vector3& position) {
    StoneObject* stone = new StoneObject(world);

    AABBVolume* volume = new AABBVolume(Vector3(0.5f, 0.5f, 0.5f));
    stone->SetBoundingVolume((CollisionVolume*)volume);

    stone->SetPlayer(player);

    stone->GetTransform()
        .SetPosition(stone->FindNearestGridCenter(position))
        .SetScale(Vector3(1, 1, 1));

    stone->SetRenderObject(new RenderObject(&stone->GetTransform(), stoneMesh, stoneTex, bumpShader));
    stone->GetRenderObject()->SetBumpTexture(stoneBumpTex);
    stone->SetPhysicsObject(new PhysicsObject(&stone->GetTransform(), stone->GetBoundingVolume()));
    stone->GetPhysicsObject()->SetGravity(false);
    stone->GetPhysicsObject()->SetResolve(false);

    stone->GetPhysicsObject()->SetInverseMass(1);
    stone->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(stone);

    return stone;
}

RailObject* TutorialGame::AddRailToWorld(const Vector3& position) {
    RailObject* rail = new RailObject(world);

    AABBVolume* volume = new AABBVolume(Vector3(0.5f, 0.5f, 0.5f));
    rail->SetBoundingVolume((CollisionVolume*)volume);

    rail->SetPlayer(player);

    rail->GetTransform()
        .SetPosition(rail->FindNearestGridCenter(position))
        .SetScale(Vector3(1, 1, 1));

    rail->SetRenderObject(new RenderObject(&rail->GetTransform(), railMesh, railTex, bumpShader));
    rail->GetRenderObject()->SetBumpTexture(railBumpTex);
    rail->SetPhysicsObject(new PhysicsObject(&rail->GetTransform(), rail->GetBoundingVolume()));
    rail->GetPhysicsObject()->SetGravity(false);
    rail->GetPhysicsObject()->SetResolve(false);

    rail->GetPhysicsObject()->SetInverseMass(1);
    rail->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(rail);

    return rail;
}

AnimalObject* TutorialGame::AddMooseToWorld(const Vector3& position) {
    AnimalObject* moose = new AnimalObject();
    AABBVolume* volume = new AABBVolume(Vector3(1.5, 1.5, 1.5));
    moose->SetBoundingVolume((CollisionVolume*)volume);

    moose->GetTransform()
        .SetPosition(position)
        .SetScale(Vector3(3, 3, 3));

    moose->SetRenderObject(new RenderObject(&moose->GetTransform(), mooseMesh, nullptr, skinningPerPixelShader, 3));
    moose->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
    moose->GetRenderObject()->SetAnimationObject(mooseAnimation);
    moose->GetRenderObject()->SetTextures(mooseTextures);

    moose->SetPhysicsObject(new PhysicsObject(&moose->GetTransform(), moose->GetBoundingVolume()));
    moose->GetPhysicsObject()->SetInverseMass(1);
    moose->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(moose);

    return moose;
}

RobotObject* TutorialGame::AddRobotToWorld(const Vector3& position) {
    RobotObject* robot = new RobotObject();
    AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));
    robot->SetBoundingVolume((CollisionVolume*)volume);

    robot->GetTransform()
        .SetPosition(position)
        .SetScale(Vector3(2, 2, 2));

    robot->SetRenderObject(new RenderObject(&robot->GetTransform(), robotMesh, nullptr, skinningBumpShader2, 3));
    robot->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
    robot->GetRenderObject()->SetAnimationObject(robotAnimation);
    robot->GetRenderObject()->SetTextures(robotTextures);
    robot->GetRenderObject()->SetBumpTextures(robotBumpTextures);

    robot->SetPhysicsObject(new PhysicsObject(&robot->GetTransform(), robot->GetBoundingVolume()));
    robot->GetPhysicsObject()->SetInverseMass(1);
    robot->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(robot);

    return robot;
}

DroneObject* TutorialGame::AddDroneToWorld(const Vector3& position) {
    DroneObject* drone = new DroneObject();
    AABBVolume* volume = new AABBVolume(Vector3(1.5, 1.5, 1.5));
    drone->SetBoundingVolume((CollisionVolume*)volume);

    drone->GetTransform()
        .SetPosition(position)
        .SetScale(Vector3(3, 3, 3));

    drone->SetRenderObject(new RenderObject(&drone->GetTransform(), droneMesh, nullptr, skinningPerPixelShader, 3));
    drone->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
    drone->GetRenderObject()->SetAnimationObject(droneAnimation);
    drone->GetRenderObject()->SetTextures(droneTextures);

    drone->SetPhysicsObject(new PhysicsObject(&drone->GetTransform(), drone->GetBoundingVolume()));
    drone->GetPhysicsObject()->SetInverseMass(1);
    drone->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(drone);

    return drone;
}

//CollectableObject* TutorialGame::AddRailToWorld(int direction,Vector3 RailPosition,Vector3 lastRailPosition)
//{
//
//
//}
//int TutorialGame::SelectRailDir(Vector3 lastRailPosition,Vector3 RailPosition)
//{
//    if(distance(lastRailPosition,RailPosition)<= 10)
//    {
//
//    }
//}

CollectableObject* TutorialGame::CreateObject(int objectId)
{


    switch (objectId)
    {
    case 1:
        //AddRailToWorld();
        break;
    case 2:
        return AddPickaxeToWorld(Vector3(20, 20, 20));
        break;
    default:

        break;
    }
    world->AddGameObject(object);

    return object;

}
