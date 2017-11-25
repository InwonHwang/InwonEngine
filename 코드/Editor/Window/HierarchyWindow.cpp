#include "HierarchyWindow.h"
#include "../IWEditor.h"

HierarchyWindow::HierarchyWindow(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size)
{
}


HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::setTreeNodeRecursively(const shared_ptr<GameObject> gameObject, int& index)
{	
	if (!ImGui::TreeNode(gameObject->GetName()->c_str())) return;

	ImGui::SameLine();

	if (ImGui::Button("Select"))
		IWEditor::SelectedObject = gameObject;

	auto transform = gameObject->GetComponent<Transform>();

	for (int i = 0; i < transform->GetChildCount(); ++i)
		setTreeNodeRecursively(transform->GetChild(i)->GetGameObject(), ++index);

	ImGui::TreePop();	
}

void HierarchyWindow::Update()
{
	if (!IsActived()) return;
	
	ImGui::Begin(_name->c_str(), 0, ImGuiWindowFlags_ShowBorders);
	
	*_pos = ImGui::GetWindowPos();
	*_size = ImGui::GetWindowSize();

	int index = 0;
	if (Scenes->GetCurrentScene() && Scenes->GetCurrentScene()->GetRootGameObject())	
		setTreeNodeRecursively(Scenes->GetCurrentScene()->GetRootGameObject(), index);

	ImGui::End();	
}
