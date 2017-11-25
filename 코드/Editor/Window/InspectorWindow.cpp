#include "InspectorWindow.h"
#include "../IWEditor.h"


InspectorWindow::InspectorWindow(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size)
{
}


InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::setModal(const string& name)
{
	if (ImGui::Button(name.c_str()))
		ImGui::OpenPopup(name.c_str());

	bool open = true;
	if (ImGui::BeginPopupModal(name.c_str(), &open))
	{
		/*static int selected = 0;
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		for (int i = 0; i < 100; i++)
		{
		char label[128];
		sprintf(label, "MyObject %d", i);
		if (ImGui::Selectable(label, selected == i))
		selected = i;
		}*/
		if (ImGui::Button("Apply"))
			ImGui::CloseCurrentPopup();

		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void InspectorWindow::setDropDown()
{
	if (!IWEditor::SelectedObject || !ImGui::CollapsingHeader("Add Component")) return;
		
	bool selected = true;

	if (ImGui::Selectable("Animator", selected))
	{
		IWEditor::SelectedObject->AddComponent<Animator>();
	}
	if (ImGui::Selectable("SkinnedMeshRenderer", selected))
	{
		IWEditor::SelectedObject->AddComponent<SkinnedMeshRenderer>();
	}
	if (ImGui::Selectable("MeshRenderer", selected))
	{
		IWEditor::SelectedObject->AddComponent<MeshRenderer>();
	}
	if (ImGui::Selectable("RigidBody", selected))
	{
		IWEditor::SelectedObject->AddComponent<RigidBody>();
	}
	if (ImGui::Selectable("BoxCollider", selected))
	{
		IWEditor::SelectedObject->AddComponent<BoxCollider>();
	}
}

void InspectorWindow::setTransformView(const shared_ptr<Transform> transform)
{
	if (!ImGui::CollapsingHeader("Transform")) return;

	auto scale = transform->GetLocalScale();
	auto rotation = transform->GetLocalEularAngle();
	auto position = transform->GetLocalPosition();

	float s[3] = { scale->x, scale->y, scale ->z};
	float r[3] = { rotation->x, rotation->y, rotation->z };
	float t[3] = { position->x, position->y, position->z };

	if(ImGui::InputFloat3("position", t)) transform->SetLocalPosition(Vector3(t[0], t[1], t[2]));
	if(ImGui::InputFloat3("rotation", r)) transform->SetLocalRotation(Quaternion::Euler(r[0], r[1], r[2]));
	if(ImGui::InputFloat3("scale", s)) transform->SetLocalScale(Vector3(s[0], s[1], s[2]));
}

void InspectorWindow::setSkinnedMeshRendererView(const shared_ptr<SkinnedMeshRenderer> skinnedMeshRenderer)
{
	if (!ImGui::CollapsingHeader("SkinnedMeshRenderer")) return;

	auto rootBone = skinnedMeshRenderer->GetRootBone();
	auto mesh = skinnedMeshRenderer->GetMesh();

	ImGui::Text("Mesh");
	ImGui::Indent();
	if(ImGui::Selectable(mesh->GetName()->c_str()))
	{
		if (ImGui::Selectable(mesh->GetName()->c_str()) && IWEditor::SelectedFile.find("mesh"))
		{
		}
	}
	ImGui::Unindent();
	ImGui::NewLine();	

	ImGui::Text("Materials");
	ImGui::Indent();
	for (int i = 0; i < skinnedMeshRenderer->GetMaterialCount(); ++i)
	{
		auto material = skinnedMeshRenderer->GetMaterial(i);

		if (ImGui::Selectable(material->GetName()->c_str()) && IWEditor::SelectedFile.find("material") != string::npos)
		{
			auto newMaterial = Resources->Create<Material>(IWEditor::SelectedFile);

			if (newMaterial) skinnedMeshRenderer->SetMaterial(i, newMaterial);
		}
	}
	ImGui::Unindent();
	ImGui::NewLine();

	ImGui::Text("RootBone");
	ImGui::Indent();
	ImGui::Text(rootBone->GetGameObject()->GetName()->c_str());
	ImGui::Unindent();
}

void InspectorWindow::setMeshRendererView(const shared_ptr<MeshRenderer> meshRenderer)
{
	if (!ImGui::CollapsingHeader("MeshRenderer")) return;

	auto mesh = meshRenderer->GetMesh();

	ImGui::Text("Mesh");
	ImGui::Indent();
	if (ImGui::Selectable(mesh->GetName()->c_str()))
	{
		if (ImGui::Selectable(mesh->GetName()->c_str()) && IWEditor::SelectedFile.find("mesh"))
		{
		}
	}
	ImGui::Unindent();
	ImGui::NewLine();

	ImGui::Text("Materials");
	ImGui::Indent();
	for (int i = 0; i < meshRenderer->GetMaterialCount(); ++i)
	{
		auto material = meshRenderer->GetMaterial(i);

		if (ImGui::Selectable(material->GetName()->c_str()) && IWEditor::SelectedFile.find("material"))
		{
			auto newMaterial = Resources->Create<Material>(IWEditor::SelectedFile);
			meshRenderer->SetMaterial(i, newMaterial);
		}
	}
	ImGui::Unindent();
	ImGui::NewLine();
}

void InspectorWindow::setAnimatorView(const shared_ptr<Animator> animator)
{
	if (!ImGui::CollapsingHeader("Animator")) return;

	ImGui::Text("Animations");
	ImGui::Indent();
	for (int i = 0; i < animator->GetAnimationCount(); ++i)
	{
		auto animation = animator->GetAnimation(i);

		if (ImGui::Selectable(animation->GetName()->c_str()) && IWEditor::SelectedFile.find(".animation"))
		{
		}
	}
	ImGui::Unindent();
	ImGui::NewLine();
}

void InspectorWindow::setRigidBodyView(const shared_ptr<RigidBody> rigidbody)
{
	if (!ImGui::CollapsingHeader("RigidBody")) return;

	float mass = rigidbody->GetMass();
	bool isKinematic = rigidbody->IsKinematic();
	bool useGravity = rigidbody->UseGravity();

	if (ImGui::Checkbox("Use Gravity", &useGravity)) rigidbody->SetGravity(useGravity);
	if (ImGui::Checkbox("Kinematic", &isKinematic)) rigidbody->SetKinematic(isKinematic);
	if (ImGui::InputFloat("mass", &mass)) rigidbody->SetMass(mass);
}

void InspectorWindow::setBoxColliderView(const shared_ptr<BoxCollider> collider)
{
	if (!ImGui::CollapsingHeader("BoxCollider")) return;

	auto extents = collider->GetExtents();

	float e[3] = { extents->x, extents->y, extents->z };

	if (ImGui::InputFloat3("extents", e)) collider->SetExtents(Vector3(e[0], e[1], e[2]));
}

void InspectorWindow::Update()
{
	if (!IsActived()) return;

	ImGui::Begin(_name->c_str(), 0, ImGuiWindowFlags_ShowBorders);
	
	*_pos = ImGui::GetWindowPos();
	*_size = ImGui::GetWindowSize();

	if (IWEditor::SelectedObject)
	{
		char buf[MAX_PATH];
		memset(buf, 0, sizeof(buf));
		
		if (ImGui::InputText(IWEditor::SelectedObject->GetName()->c_str(), buf, MAX_PATH))
		{
			IWEditor::SelectedObject->SetName(buf);
		}

		auto components = IWEditor::SelectedObject->GetComponents<ComponentBase>();

		for (auto component : *components) {
			if (typeid(*component) == typeid(Transform)) setTransformView(static_pointer_cast<Transform>(component));
			else if (typeid(*component) == typeid(Animator)) setAnimatorView(static_pointer_cast<Animator>(component));
			else if (typeid(*component) == typeid(SkinnedMeshRenderer)) setSkinnedMeshRendererView(static_pointer_cast<SkinnedMeshRenderer>(component));
			else if (typeid(*component) == typeid(MeshRenderer)) setMeshRendererView(static_pointer_cast<MeshRenderer>(component));
			else if (typeid(*component) == typeid(RigidBody)) setRigidBodyView(static_pointer_cast<RigidBody>(component));
			else if (typeid(*component) == typeid(BoxCollider)) setBoxColliderView(static_pointer_cast<BoxCollider>(component));
		}
	}

	setDropDown();
	
	ImGui::End();
}
