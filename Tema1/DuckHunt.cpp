#include "DuckHunt.h"
#include <iostream>
#include <math.h>

#include "Figure.h"
#include "Operations.h"
#include <random>

using namespace std;
using namespace tema1;

std::random_device rd;
std::mt19937 gen(rd());


Tema1::Tema1() {
    // initial score
    scorePoints = 0;
    scoreSlider = 0.0;
    lives = 3;
    bullets = 0;

    terrainSize = { 10, 5 };

    spawnDuck = true;
    duckPosition = { 0, 0 };

    maxEvasionTime = 8.0;

    distancePerUnit = 1.2;
    movementSpeed = 1;

    wingsUp = true;
    animationDuration = 10;
}

Tema1::~Tema1() {
}

void Tema1::Init() {

    // set window resolution and logic space params
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 5));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = 5;
    logicSpace.height = 5;

    // add duck mesh
    Mesh* duck_body = geometry::CreateRectangle("duck_body",  glm::vec3(0, 1, 0), 1, 1, glm::vec3(0.4, 0.2, 0.2), true);
    Mesh* duck_wing1 = geometry::CreateRectangle("duck_wing1", glm::vec3(-0.5, 1.25, 0), 0.5, 0.5, glm::vec3(0.4, 0.5, 0.6), true);
    Mesh* duck_wing2 = geometry::CreateRectangle("duck_wing2", glm::vec3(1, 1.25, 0), 0.5, 0.5, glm::vec3(0.4, 0.5, 0.6), true);
    Mesh* duck_head = geometry::CreateTriangle("duck_head", glm::vec3(0.25, 2, 0), 0.5, glm::vec3(0.8, 0.85, 0), true);
    AddMeshToList(duck_wing1);
    AddMeshToList(duck_wing2);
    AddMeshToList(duck_body);
    AddMeshToList(duck_head);

    // add grass mesh
    Mesh* square = geometry::CreateSquare("terrain", glm::vec3(-2, -9, 0), terrainSize.first, glm::vec3(0.5, 0.55, 0.2), true);
    AddMeshToList(square);

    // add life UI mesh
    Mesh* circle = geometry::CreateCircle("life", 0.2, 5, 4.5, glm::vec3(0.9, 0.3, 0.3), true);
    AddMeshToList(circle);

    // add bullet UI mesh
    Mesh* bullet = geometry::CreateRectangle("bullet", glm::vec3(4.9, 3.8, 0), 0.2, 0.3, glm::vec3(0.7, 0.7, 0.2), false);
    AddMeshToList(bullet);

    // add score stroke mesh
    Mesh* scorebar = geometry::CreateRectangle("score_bar", glm::vec3(-2, 4, 0), 2, 0.5, glm::vec3(0, 0, 0), false);
    AddMeshToList(scorebar);

    // add score that will fill the bar
    Mesh* score = geometry::CreateRectangle("score", glm::vec3(-4, 4, 0), 2, 0.5, glm::vec3(0.3, 0.3, 0.8), true);
    AddMeshToList(score);

}

void Tema1::FrameStart() {

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Tema1::FrameEnd() {
}

void Tema1::Update(float deltaTimeSeconds) {

    // check player has no more lives
    if (lives <= 0) {
        exit(0);
    }

    // set viewport and resolution
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.4), true);

    // uniform 2D visualization matrix
    modelMatrix = glm::mat3(1);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    modelMatrix = glm::mat3(1);

    // draw terrain grass
    modelMatrix = visMatrix * Operations::Translate(0, 0);
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], modelMatrix);

    DrawUI();

    DrawDuck(deltaTimeSeconds, visMatrix);
}

void Tema1::DrawUI() {

    // render health points
    for (int i = 0; i < lives; i++) {
        modelMatrix = visMatrix * Operations::Translate((float)i / 2, 0);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    // render bullets
    for (int i = 0; i < bullets; i++) {
        modelMatrix = visMatrix * Operations::Translate((float)i / 2, 0);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    // render score
    modelMatrix = visMatrix * Operations::Translate(0, 0);
    RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * Operations::Translate(scoreSlider, 0);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);
}

// increase score points
void Tema1::IncreaseScore() {
    scorePoints++;
    // move score slider only for 20 points max
    if (scorePoints <= 20) {
        scoreSlider += 0.1;
    }
    if (scorePoints % 5 == 0) {
        cout << "SCORE = " << scorePoints << endl;
    }
}

// function that generates random value between low and high
int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

void Tema1::CalculateDuckDirection() {
    // check if duck hits the right margin of screen
    if (duckPosition.first >= logicSpace.width + 0.5) {
        if (direction == 1)
            direction = 0;
        if (direction == 2)
            direction = 3;
    }
    // check if duck hits the left margin of screen
    if (duckPosition.first <= logicSpace.x - 1.5) {
        if (direction == 0)
            direction = 1;
        if (direction == 3)
            direction = 2;
    }

    // check if duck hits the upper margin of screen
    if (duckPosition.second >= logicSpace.height - 2.5) {
        if (direction == 1)
            direction = 2;
        if (direction == 0)
            direction = 3;
    }

    // check if duck hits the lower margin of screen
    if (duckPosition.second <= logicSpace.y - 2) {
        if (direction == 3)
            direction = 0;
        if (direction == 2)
            direction = 1;
    }
}

void Tema1::MoveDuck(float deltaTimeSeconds) {
    switch (direction) {
        // move duck diagonally like from bottom to upper left
    case 0: {
        duckPosition.first -= distancePerUnit * deltaTimeSeconds * movementSpeed;
        duckPosition.second += distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
        // move duck diagonally from left to upper right
    case 1: {
        duckPosition.first += distancePerUnit * deltaTimeSeconds * movementSpeed;
        duckPosition.second += distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
        // move duck diagonally from upper margin to lower right
    case 2: {
        duckPosition.first += distancePerUnit * deltaTimeSeconds * movementSpeed;
        duckPosition.second -= distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
        // move duck diagonally from left to lower right
    case 3: {
        duckPosition.first -= distancePerUnit * deltaTimeSeconds * movementSpeed;
        duckPosition.second -= distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
        // move duck up (escapes)
    case 4: {
        duckPosition.second += distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
        // move duck down (falls)
    case 5: {
        duckPosition.second -= distancePerUnit * deltaTimeSeconds * movementSpeed;
        break;
    }
    }
}

bool Tema1::CheckEvasion(float deltaTimeSeconds) {
    if (currentEvasionTime >= maxEvasionTime || bullets <= 0) {
        direction = 4;
        currentEvasionTime = 0;
        lives -= 1;
        isDuckEvaded = true;
        return true;
    }
    else {
        currentEvasionTime += 1 * deltaTimeSeconds;
        return false;
    }
}

void Tema1::ResetDuckProperties() {
    // increase duck speed
    movementSpeed += 0.2;

    // random choose duck position for spawn
    duckPosition.first = random(0, 3);
    duckPosition.second = -2;
    spawnDuck = false;

    // random choose direction
    direction = random(0, 1);

    // reset evasion time and bullets
    currentEvasionTime = 0.0;
    isDuckEvaded = false;
    bullets = 3;
}

void Tema1::RenderDuckMesh(float deltaTimeSeconds, glm::mat3 visMatrix) {

    // check for wings animation
    if (animationDuration <= 0) {
        wingsUp = !wingsUp;
        animationDuration = 10;
    }
    else {
        animationDuration--;
    }

    // draw duck
    modelMatrix = visMatrix * Operations::Translate(duckPosition.first, duckPosition.second);
    RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], modelMatrix);

    if (wingsUp) {
        modelMatrix = visMatrix
            * Operations::Translate(duckPosition.first, duckPosition.second)
            * Operations::Scale(0.5, 1)
            * Operations::Translate(0, 0);
        RenderMesh2D(meshes["duck_wing1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix
            * Operations::Translate(duckPosition.first, duckPosition.second)
            * Operations::Scale(0.5, 1)
            * Operations::Translate(1, 0);
        RenderMesh2D(meshes["duck_wing2"], shaders["VertexColor"], modelMatrix);
    }
    else {
        modelMatrix = visMatrix
            * Operations::Translate(duckPosition.first, duckPosition.second)
            * Operations::Scale(1, 1)
            * Operations::Translate(0, 0);
        RenderMesh2D(meshes["duck_wing1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix
            * Operations::Translate(duckPosition.first, duckPosition.second)
            * Operations::Scale(1, 1)
            * Operations::Translate(0, 0);
        RenderMesh2D(meshes["duck_wing2"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * Operations::Translate(duckPosition.first, duckPosition.second);
    RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawDuck(float deltaTimeSeconds, glm::mat3 visMatrix) {

    // check if duck should respawn
    if (spawnDuck) {
        ResetDuckProperties();
    }
    else {
        int delayTime = -1;
        // check if duck left the screen when evaded
        if (duckPosition.second + delayTime > logicSpace.height) {
            spawnDuck = true;
        }
        // check if duck left the screen when fallen
        if (duckPosition.second + 1.5 < logicSpace.x && isDuckFalling) {
            spawnDuck = true;
            isDuckFalling = false;
        }
        // change duck direction only if it is not evaded
        if (!isDuckEvaded) {
            CalculateDuckDirection();
            CheckEvasion(deltaTimeSeconds);
        }
        // move duck on screen
        MoveDuck(deltaTimeSeconds);
    }

    // draw duck
    RenderDuckMesh(deltaTimeSeconds, visMatrix);
}

void Tema1::ShootDuck() {
    // offsets of mouse points being converted to logic space
    float mouseOffset = 0.4;
    float realMouseX = mousePosition.first + mouseOffset;
    float realMouseY = mousePosition.second + mouseOffset;
    float duckOffsetX = 0.9;
    float duckOffsetY = 0.9;
    float minDuckPositionX = duckPosition.first - duckOffsetX;
    float maxDuckPositionX = duckPosition.first + duckOffsetX;
    float minDuckPositionY = duckPosition.second - duckOffsetY;
    float maxDuckPositionY = duckPosition.second + duckOffsetY;

    if (!isDuckFalling) {
        // check duck collision
        if (realMouseX >= minDuckPositionX && realMouseX <= maxDuckPositionX &&
            realMouseY >= minDuckPositionY && realMouseY <= maxDuckPositionY) {
            direction = 5;
            isDuckFalling = true;
            IncreaseScore();
        }
    }
    
    bullets--;
}


void tema1::Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (!isDuckEvaded) {
        ShootDuck();
    }

    //IGNORE: DEBUG INFO
    /*cout << "mouse: " << realMouseX << " | " << realMouseY << "\n" << 
        "Duck X interval: " << minDuckPositionX << " | " << maxDuckPositionX << "\n" <<
        "Duck Y interval: " << minDuckPositionY << " | " << maxDuckPositionY << "\n";*/
}

glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::ConvertMousePositionToLocalSpace(int mouseX, int mouseY) {
    float intervalInversionFactor = 2.2;
    mousePosition.first = (float)mouseX * (logicSpace.width - (logicSpace.x - 2)) / viewSpace.width + (logicSpace.x - 2);
    mousePosition.second = (float)mouseY * (logicSpace.height - (logicSpace.y - 2)) / viewSpace.height + (logicSpace.y -2);
    mousePosition.second = intervalInversionFactor - mousePosition.second;
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    ConvertMousePositionToLocalSpace(mouseX, mouseY);

}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

}

void Tema1::OnInputUpdate(float deltaTime, int mods) {

}

void Tema1::OnKeyPress(int key, int mods) {
    //IGNORE: FOR DEBUG POURPOSES
    //if (key == GLFW_KEY_0)
    //    spawnDuck = true;
}

void Tema1::OnKeyRelease(int key, int mods) {

}