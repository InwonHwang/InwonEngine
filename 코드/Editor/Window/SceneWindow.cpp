#include "SceneWindow.h"
#include "../IWEditor.h"

SceneWindow::SceneWindow(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size)
{
}


SceneWindow::~SceneWindow()
{
}

void SceneWindow::Update()
{
	if (!IsActived()) return;
	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin(_name->c_str(), 0, ImGuiWindowFlags_ShowBorders);

	*_pos = ImGui::GetWindowPos();
	*_size = ImGui::GetWindowSize();

	ImGui::Text(IWEditor::SelectedFile.c_str());

	ImGui::End();
	ImGui::PopStyleColor();
}
