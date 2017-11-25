#include "ProjectWindow.h"
#include "../IWEditor.h"
#include "../FBXParser.h"

ProjectWindow::ProjectWindow(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size),
	_selected(-1),
	_fbxParser(make_unique<FBXParser>())
{	
}


ProjectWindow::~ProjectWindow()
{	
}

void ProjectWindow::setDirectoryList(const experimental::filesystem::path& dir)
{
	if (!experimental::filesystem::is_directory(dir)) return;

	if (!ImGui::TreeNode(dir.filename().generic_string().c_str())) return;

	ImGui::SameLine();

	if (ImGui::Button("Select"))
		IWEditor::SelectedDirectory = dir.generic_string().c_str();

	experimental::filesystem::directory_iterator it(dir);

	for (auto sub_dir : it)	
		setDirectoryList(sub_dir);

	ImGui::TreePop();
}


void ProjectWindow::setFileList()
{
	if (IWEditor::SelectedDirectory.empty()) return;

	ImGui::Text(IWEditor::SelectedDirectory.c_str());
	ImGui::Separator();

	experimental::filesystem::directory_iterator it(IWEditor::SelectedDirectory.c_str());

	int i = 0;
	for (auto file : it)
	{
		if (experimental::filesystem::is_directory(file)) continue;

		if (ImGui::Selectable(file.path().filename().generic_string().c_str(), _selected == i)) {
			IWEditor::SelectedFile = file.path().generic_string().c_str();
			_selected = i;

			if (IWEditor::SelectedFile.find(".fbx") != string::npos || IWEditor::SelectedFile.find(".FBX") != string::npos)
			{
				_fbxParser->ParseAndExport(IWEditor::SelectedFile);
			}
			else if (IWEditor::SelectedFile.find(".prefab") != string::npos)
			{
				auto newObj = Resources->Create<GameObject>(IWEditor::SelectedFile);
				newObj->GetComponent<Transform>()->SetParent(Scenes->GetCurrentScene()->GetRootGameObject()->GetComponent<Transform>());
				IWEditor::SelectedFile = "";
			}
			else if (IWEditor::SelectedFile.find(".skybox") != string::npos)
			{
				auto skyBox = Resources->Create<SkyBox>(IWEditor::SelectedFile);
				Scenes->GetCurrentScene()->SetSkyBox(skyBox);
				IWEditor::SelectedFile = "";
			}
		}

		i++;
	}
}

void ProjectWindow::Update()
{
	if (!IsActived()) return;

	ImGui::Begin(_name->c_str(), 0, ImGuiWindowFlags_ShowBorders);
	
	*_pos = ImGui::GetWindowPos();
	*_size = ImGui::GetWindowSize();

	ImGui::BeginChild("left pane", ImVec2(500, 0), true);
	setDirectoryList(".\\Assets");
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginGroup();
	setFileList();
	ImGui::EndGroup();

	ImGui::End();
}
