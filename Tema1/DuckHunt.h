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

        // UI and gameplay functions
        void IncreaseScore();

        void CalculateDuckDirection();
        void MoveDuck(float deltaTimeSeconds);
        bool CheckEvasion(float deltaTimeSeconds);
        void ResetDuckProperties();

        // draw duck on screen
        void DrawCharacter(float deltaTimeSeconds, glm::mat3 visMatrix);
        void RenderDuckMesh(float deltaTimeSeconds, glm::mat3 visMatrix);
        
        void ConvertMousePositionToLocalSpace(int mouseX, int MouseY);

        void ShootDuck();


    protected:

        int scorePoints;
        float scoreSlider;
        int lives;
        int bullets;

        bool spawnDuck;
        bool isDuckFalling;
        bool isDuckEvaded;
        bool wingsUp;
        int  animationDuration;

        std::pair<float, float> duckPosition;
        std::pair<float, float> mousePosition;

        int direction;
        float distancePerUnit;
        float movementSpeed;

        float maxEvasionTime;
        float currentEvasionTime;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        std::pair<float, float> terrainSize;

        Healthbar playerHealth;
        float healthBarOffset;
    };
};