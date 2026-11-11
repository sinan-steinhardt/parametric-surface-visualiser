#include <glm/glm.hpp>
#include "raylib.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <numbers>
#include <math.h>
#include <regex>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "muParser.h"

std::pair<float,float> extractBounds(const std::string& range) {
    float a, b;
    char open, comma, close;
    std::stringstream ss(range);

    if (ss >> open >> a >> comma >> b >> close) {
        if (open == '[' && comma == ',' && close == ']' && a < b) {
            return {a, b};
        }
    }
    throw std::invalid_argument("Invalid range format. Please use [a,b] where a < b.");

}

int main() {
    //Get user info
    std::string XofTL;
    std::cout << "Please insert your parametrisations as a function of t and l below:" << std::endl;
    std::cout << "x(t,l) = ... " << std::endl;
    std::cin >> XofTL;

    std::string YofTL;
    std::cout << "y(t,l) = ... " << std::endl;
    std::cin >> YofTL;

    std::string ZofTL;
    std::cout << "z(t,l) = ... " << std::endl;
    std::cin >> ZofTL;

    std::cout << "" << std::endl;

    std::string tRange;
    std::cout << "Please insert your parameter ranges in the desired format below:" << std::endl;
    std::cout << "for t" << std::endl;
    std::cout << "[a,b] = ... " << std::endl;
    std::cin >> tRange;

    std::string lRange;
    std::cout << "for l" << std::endl;
    std::cout << "[a,b] = ... " << std::endl;
    std::cin >> lRange;

    mu::Parser parserX;
    mu::Parser parserY;
    mu::Parser parserZ;

    double mu_t = 0.0;
    double mu_l = 0.0;

    parserX.DefineVar("t", &mu_t);
    parserX.DefineVar("l", &mu_l);
    parserY.DefineVar("t", &mu_t);
    parserY.DefineVar("l", &mu_l);
    parserZ.DefineVar("t", &mu_t);
    parserZ.DefineVar("l", &mu_l);

    parserX.SetExpr(XofTL);
    parserY.SetExpr(YofTL);
    parserZ.SetExpr(ZofTL);

    //Define Point Path, Time, Lime (uses glm::vec3 for glm calculations)
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    std::vector<glm::vec3> path;
    bool TorL = true;
    bool increasing = true;
    bool firstRun = true;
    float tMax = extractBounds(tRange).second;
    float tMin = extractBounds(tRange).first;
    float lMax = extractBounds(lRange).second;
    float lMin = extractBounds(lRange).first;
    float t = tMin;
    float l = lMin;
    float tspeed = (tMax - tMin)/5; //1 seconds to go from min to max
    float lspeed = (lMax - lMin)/5; //1 seconds to go from min to max

    //Define Window
    InitWindow(1000, 800, "3DSurface");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    //Set up Window
    SetTargetFPS(60);

    //Define Camera
    Camera3D camera = {{0}};

    //Set up Camera (uses Vector3 for raylib.h rendering)
    camera.position = Vector3{5.0f, 5.0f, 5.0f};
    camera.target = Vector3{0, 0, 0};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraAngle = 0.0f;
    float cameraRadius = 4.0f;
    float cameraSpeed = 0.08f;
    float cameraZoomSpeed = 0.05f;

    //Running and Rendering
    while(!WindowShouldClose()) {

    if (tMin >= tMax || lMin >= lMax) {
        break;
    }
    
        //Get global time after frame update
        auto now = std::chrono::steady_clock::now();

        //Check if switching between t and l is needed based on its value
        if (TorL == true) {
            //t changes and l is constant
            if (increasing == true && firstRun == true) {
                t += tspeed * GetFrameTime();

                if (t >= tMax) {
                    t = tMax;
                    TorL = false;
                    firstRun = false;
                }
            }

            else if (increasing == true && firstRun == false) {
                t += tspeed * GetFrameTime();

                if (t >= tMax) {
                    t = tMax;
                    TorL = false;
                    lMax -= 2 * lspeed * GetFrameTime();
                }
            }

            else if (increasing == false && firstRun == false) {
                t -= tspeed * GetFrameTime();

                if (t <= tMin) {
                    t = tMin;
                    TorL = false;
                    lMin += 2 * lspeed * GetFrameTime();

                }

            }

        }

        else if (TorL == false) {
            //l changes and t is constant
            if (increasing == true && firstRun == false) {
                l += lspeed * GetFrameTime();

                if (l >= lMax) {
                    l = lMax;
                    TorL = true;
                    increasing = false;
                    tMin += 2 * tspeed * GetFrameTime();
                }
            }

            else if (increasing == false && firstRun == false) {
                l -= lspeed * GetFrameTime();

                if (l <= lMin) {
                    l = lMin;
                    TorL = true;
                    increasing = true;
                    tMax -= 2 * tspeed * GetFrameTime();
                }

            }

        }
    
        mu_t = static_cast<double>(t);
        mu_l = static_cast<double>(l);

        //Update point positions based on parametrisation
        point.x = static_cast<float>(parserX.Eval());;
        point.y = static_cast<float>(parserY.Eval());;
        point.z = static_cast<float>(parserZ.Eval());;

        //Update path based on point
        path.push_back(point);

        //Direct Camera
        cameraAngle += GetFrameTime() * cameraSpeed;
        float mouseWheel = GetMouseWheelMove();
        cameraRadius -= mouseWheel * cameraZoomSpeed;

        camera.position.x = cameraRadius * cos(cameraAngle);
        camera.position.y = cameraRadius * sin(cameraAngle);
        camera.position.z = cameraRadius;

        //Render
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawSphere(Vector3{point.x, point.y, point.z}, 0.01f, GRAY);

                for(int i = 0; i < path.size() - 1; i++) {
                    DrawLine3D(Vector3{path[i].x, path[i].y, path[i].z}, Vector3{path[i+1].x, path[i+1].y, path[i+1].z}, RED);
                }

            EndMode3D();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}