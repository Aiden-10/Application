#include "UI.h"

#include "helper.hpp"
#include "UIs/settings.hpp"

void UI::Render()
{
    settings::render();

	DrawRadar();
}

void UI::DrawRadar()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 WindowPos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y);
    ImVec2 windowSizeLast = windowSize;
    windowSize = viewport->WorkSize;
    
    if (windowSize.x != windowSizeLast.x || windowSize.x != windowSizeLast.x)
    {
        offset = { 0, 0 };
    }

	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always);
	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	radarHovered = ImGui::IsWindowHovered();
	RadarMousePos = { io.MousePos.x - ImGui::GetWindowPos().x, io.MousePos.y - ImGui::GetWindowPos().y };

	unsigned int image_dimension_x;
	unsigned int image_dimension_y;

	std::string path = "assets\\maps\\example.png";
	helper::get_png_image_dimensions(path, image_dimension_x, image_dimension_y);

	ImGui::SetCursorPos(offset);
	ImGui::Image(radarImage, ImVec2(image_dimension_x * zoom, image_dimension_y * zoom));

    DrawSettingsButton();

	HandleRadar();
}

void UI::HandleRadar()
{
    ImGuiIO& io = ImGui::GetIO();

    if (radarHovered)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            if (!isMouseDragging) {
                // Mouse button was just pressed
                isMouseDragging = true;
                mouseDownPos = io.MousePos;
            }
            else {
                // Mouse button is being held down, update the image position
                ImVec2 currentMousePos = io.MousePos;
                ImVec2 delta = ImVec2(currentMousePos.x - mouseDownPos.x, currentMousePos.y - mouseDownPos.y);
                offset.x += delta.x;
                offset.y += delta.y;
                mouseDownPos = currentMousePos;
            }
        }
        else {
            if (isMouseDragging) {
                isMouseDragging = false;
            }
        }

        if (io.MouseWheel != 0) {

            if (io.MouseWheel > 0)
            {

                if (zoom < maxZoom)
                {
                    zoom *= 1.1;

                    offset.x -= (RadarMousePos.x - offset.x) * (1.1 - 1);
                    offset.y -= (RadarMousePos.y - offset.y) * (1.1 - 1);
                }
            }
            else
            {
                if (zoom > minZoom)
                {
                    zoom *= 1 / 1.1;
                    offset.x -= (RadarMousePos.x - offset.x) * (1 / 1.1 - 1);
                    offset.y -= (RadarMousePos.y - offset.y) * (1 / 1.1 - 1);
                }
            }
        }
    }
}

void UI::DrawSettingsButton()
{
    
}

