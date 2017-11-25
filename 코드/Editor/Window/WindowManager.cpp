#include "WindowManager.h"
#include "Window.h"
#include "../stdafx.h"
#include "../IWEditor.h"

WindowManager::WindowManager(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size),
	_windows(make_unique<WindowMap>())
{
	Add(make_shared<HierarchyWindow>("Hierarchy", ImVec2(0, 20), ImVec2(300, 500)));
	Add(make_shared<SceneWindow>("Scene", ImVec2(0, 20), ImVec2(300, 500)));
	Add(make_shared<InspectorWindow>("Inspector", ImVec2(0, 20), ImVec2(300, 500)));
	Add(make_shared<ProjectWindow>("Project", ImVec2(0, 20), ImVec2(300, 500)));
}


WindowManager::~WindowManager()
{
}

void WindowManager::Update()
{
	
	//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	//{
	//	static float f = 0.0f;
	//	ImGui::Text("Hello, world!");
	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	//	//ImGui::ColorEdit3("clear color", (float*)&clear_color);
	//	//if (ImGui::Button("Another Window")) show_another_window ^= 1;
	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//}

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				IWEditor::CreateScene();
			}

			if (ImGui::MenuItem("Save Scene"))
			{
			}

			if (ImGui::MenuItem("Open Scene"))
			{
				ImGui::OpenPopup("Open Scene");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Hierarchy")) (*_windows)["Hierarchy"]->SetActive(true);
			if (ImGui::MenuItem("Scene")) (*_windows)["Scene"]->SetActive(true);
			if (ImGui::MenuItem("Inspector")) (*_windows)["Inspector"]->SetActive(true);
			if (ImGui::MenuItem("Project")) (*_windows)["Project"]->SetActive(true);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Resource"))
		{
			if (ImGui::MenuItem("LoadPrefab"))
			{
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("CreateCamera"))
				IWEditor::CreateCamera();

			if (ImGui::MenuItem("CreateGameObject"))
				IWEditor::CreateGameObject();


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
	ImGui::PopStyleColor();

	for (auto pair : *_windows)
	{
		pair.second->Update();
	}
}
