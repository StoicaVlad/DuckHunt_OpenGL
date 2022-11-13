#include "components/simple_scene.h"
#include "Objects.h"
#include <vector>
#include <utility>
#include <string>
#include <chrono>
#include <ctime>

namespace tema1 {
    class Tema1 : public gfxc::SimpleScene {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

        // Increase score
        void IncreaseScore();

        // Caclculate in which direction the duck moves
        void CalculateDuckDirection();

        // Move duck on screen
        void MoveDuck(float deltaTimeSeconds);

        // draw duck on screen
        void DrawDuck(float deltaTimeSeconds, glm::mat3 visMatrix);
        void RenderDuckMesh(float deltaTimeSeconds, glm::mat3 visMatrix);

        // Check if duck has evaded
        bool CheckEvasion(float deltaTimeSeconds);

        // Reset variables when a new duck spawns
        void ResetDuckProperties();

        // Render UI elements
        void DrawUI();
        
        // Convert mouse position in pixels to local space
        void ConvertMousePositionToLocalSpace(int mouseX, int MouseY);
        
        // Shoot duck on screen
        void ShootDuck();


    protected:

        // score points and score slider(how much the score bar should move)
        int scorePoints;
        float scoreSlider;
        
        // no lives and bullets
        int lives;
        int bullets;

        // variable to spawn another duck
        bool spawnDuck;
        // check if duck is falling
        bool isDuckFalling;
        // check if duck is evaded
        bool isDuckEvaded;
        // check how the wings animation should look like
        bool wingsUp;
        int  animationDuration;

        std::pair<float, float> duckPosition;
        std::pair<float, float> mousePosition;

        std::pair<float, float> terrainSize;

        // direction the duck moves
        int direction;
        // how much the duck moves in an unit of time
        float distancePerUnit;
        // duck speed
        float movementSpeed;

        // max time before duck evades and current time since it spawned
        float maxEvasionTime;
        float currentEvasionTime;

        // logicSpace and viewportSapce structures
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
    };
};