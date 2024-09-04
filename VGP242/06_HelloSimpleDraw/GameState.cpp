#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

const char* gDrawTypeNames[] =
{
	"None",
	"Cone",
	"Line",
	"Sphere",
	"AABB",
	"AABBFilled",
	"Oval",
	"OvalFilled",
	"Pigeon"
};

struct pigeonCone
{
	// positions
	Vector3 circleCenter;
	Vector3 coneTip;
	Color color;
	// circle
	int slices;
	float radius;

	pigeonCone()
	{
		circleCenter = Vector3::Zero;
		coneTip = Vector3::YAxis;

		slices = 30;
		radius = 2.0f;
		color = Colors::MediumPurple;
	}
};

struct Sphere
{
	int slices;
	int rings;
	float radius;
	Vector3 sphereCenter;
	Color color;

	Sphere()
	{
		slices = 30;
		rings = 30;
		radius = 2.0f;
		sphereCenter = Vector3::Zero;

		color = Colors::MediumPurple;
	}
};

struct Oval
{
	int slices;
	int rings;
	float rx, ry, rz;

	Vector3 center;
	Color color;

	Oval()
	{
		slices = 30;
		rings = 30;
		rx = ry = rz = 2.0f;

		center = Vector3::Zero;
		color = Colors::LightCoral;
	}
};

struct Pigeon
{
    // Face
    Sphere face;
    Oval leftEye, rightEye;
    Oval leftPupil, rightPupil;

    // Body
    Oval upperBody, lowerBody;

    // Arms
    Oval leftArm, rightArm;
    Sphere leftHand, rightHand;

    // Legs
    Oval leftLeg, rightLeg;
    Oval leftFoot, rightFoot;

    // Tail
    Oval tailBase, tailMid, tailTip;

    // Spikes
    std::vector<pigeonCone> cones;
    Color coneColor;

    Pigeon()
    {
        coneColor = Colors::Goldenrod;
    }
};

Pigeon pigeon;
void Createpigeon(Pigeon& pigeon)
{
	// Eyes
	// Left Eye
	pigeon.leftEye.center = { 0.6f, 0.7f, -1.5f };
	pigeon.leftEye.rx = 0.25f;
	pigeon.leftEye.ry = 0.4f;
	pigeon.leftEye.rz = 0.15f;
	pigeon.leftEye.color = { 70, 130, 180, 255 };

	// Right Eye
	pigeon.rightEye.center = { -0.6f, 0.7f, -1.5f };
	pigeon.rightEye.rx = 0.25f;
	pigeon.rightEye.ry = 0.4f;
	pigeon.rightEye.rz = 0.15f;
	pigeon.rightEye.color = { 70, 130, 180, 255 };

	// Left Pupil
	pigeon.leftPupil.center = { 0.6f, 0.6f, -1.6f }; // Adjusted Y position to 0.6f
	pigeon.leftPupil.rx = 0.08f;
	pigeon.leftPupil.ry = 0.15f;
	pigeon.leftPupil.rz = 0.1f;
	pigeon.leftPupil.color = { 0, 0, 0, 255 };

	// Right Pupil
	pigeon.rightPupil.center = { -0.6f, 0.6f, -1.6f }; // Adjusted Y position to 0.6f
	pigeon.rightPupil.rx = 0.08f;
	pigeon.rightPupil.ry = 0.15f;
	pigeon.rightPupil.rz = 0.1f;
	pigeon.rightPupil.color = { 0, 0, 0, 255 };

	// Mouth
	pigeon.cones.clear();
	pigeonCone mouth;
	mouth.circleCenter = { 0.0f, 0.0f, -1.5f };
	mouth.coneTip = { 0.0f, -0.8f, -1.5f };
	mouth.radius = 0.3f;
	mouth.slices = 7;
	pigeon.cones.push_back(mouth);
}

void Drawpigeon(const Pigeon& pigeon)
{
	// Face
	SimpleDraw::AddFilledSphere(pigeon.face.slices, pigeon.face.rings, pigeon.face.radius, pigeon.face.sphereCenter, pigeon.face.color);

	// Eyes
	SimpleDraw::AddFilledOval(pigeon.leftEye.slices, pigeon.leftEye.rings, pigeon.leftEye.rx, pigeon.leftEye.ry, pigeon.leftEye.rz, pigeon.leftEye.center, pigeon.leftEye.color);
	SimpleDraw::AddFilledOval(pigeon.rightEye.slices, pigeon.rightEye.rings, pigeon.rightEye.rx, pigeon.rightEye.ry, pigeon.rightEye.rz, pigeon.rightEye.center, pigeon.rightEye.color);

	// Pupils
	SimpleDraw::AddFilledOval(pigeon.leftPupil.slices, pigeon.leftPupil.rings, pigeon.leftPupil.rx, pigeon.leftPupil.ry, pigeon.leftPupil.rz, pigeon.leftPupil.center, pigeon.leftPupil.color);
	SimpleDraw::AddFilledOval(pigeon.rightPupil.slices, pigeon.rightPupil.rings, pigeon.rightPupil.rx, pigeon.rightPupil.ry, pigeon.rightPupil.rz, pigeon.rightPupil.center, pigeon.rightPupil.color);

	// Mouth
	for (const auto& cone : pigeon.cones)
	{
		SimpleDraw::AddCone(cone.slices, cone.radius, cone.circleCenter, cone.coneTip, pigeon.coneColor);
	}
}


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	Createpigeon(pigeon);
}

void GameState::Terminate()
{
	
}


void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

Vector3 minExtents = -Vector3::One;
Vector3 maxExtents = Vector3::One;

float radius = 10.0f;
int slices = 10;
int rings = 10;
float rx = 5.0f;
float ry = 5.0f;
float rz = 5.0f;

Color lineColor = Colors::Green;

void GameState::Render()
{
	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		SimpleDraw::AddSphere(30, 30, 2.0f, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Cone)
	{
		SimpleDraw::AddCone(slices, radius, minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Line)
	{
		SimpleDraw::AddLine(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::AABB)
	{
		SimpleDraw::AddAABB(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::AABBFilled)
	{
		SimpleDraw::AddFilledAABB(minExtents, maxExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Oval)
	{
		SimpleDraw::AddOval(slices, rings, rx, ry, rz, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::OvalFilled)
	{
		SimpleDraw::AddFilledOval(slices, rings, rx, ry, rz, minExtents, lineColor);
	}

	else if (mDebugDrawType == DebugDrawType::Pigeon)
	{
		Drawpigeon(pigeon);
	}
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	int currentDrawType = static_cast<int>(mDebugDrawType);
	if (ImGui::Combo("DrawType", &currentDrawType, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mDebugDrawType = (DebugDrawType)currentDrawType;
	}

	if (mDebugDrawType == DebugDrawType::Sphere)
	{
		// ImGui::DragFloat("SphereRadius", &radius, 0.1f, 0.01f, 1000000.0f);
		ImGui::DragFloat3("Pos", &minExtents.x);
	}
	
	else if (mDebugDrawType == DebugDrawType::Cone)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat3("Cone Tip", &maxExtents.x);
		ImGui::DragFloat("Circle Radius", &radius);
		ImGui::DragInt("Slices", &slices);
	}

	else if (mDebugDrawType == DebugDrawType::Line)
	{
		ImGui::DragFloat3("LineStart", &minExtents.x);
		ImGui::DragFloat3("LineEnd", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::AABB)
	{
		ImGui::DragFloat3("minExtent", &minExtents.x);
		ImGui::DragFloat3("maxExtent", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::AABBFilled)
	{
		ImGui::DragFloat3("minExtent", &minExtents.x);
		ImGui::DragFloat3("maxExtent", &maxExtents.x);
	}

	else if (mDebugDrawType == DebugDrawType::Pigeon)
	{
		// Head
		if (ImGui::CollapsingHeader("Head"))
		{
			// Face
			if (ImGui::CollapsingHeader("Face"))
			{
				//ImGui::Unindent();
				ImGui::DragFloat3("Face Pos", &pigeon.face.sphereCenter.x, 0.1f);
				ImGui::Indent();
				ImGui::ColorEdit4("Face Color", &pigeon.face.color.r);
				ImGui::Indent();
				ImGui::DragFloat("Face Radius", &pigeon.face.radius, 0.1f);
			}

			// Eyes
			if (ImGui::CollapsingHeader("Eyes"))
			{
				if (ImGui::CollapsingHeader("Left Eye"))
				{
					ImGui::DragFloat3("Left Eye", &pigeon.leftEye.center.x, 0.1f);
					ImGui::DragFloat("Left Eye Rx", &pigeon.leftEye.rx, 0.1f);
					ImGui::DragFloat("Left Eye Ry", &pigeon.leftEye.ry, 0.1f);
					ImGui::DragFloat("Left Eye Rz", &pigeon.leftEye.rz, 0.1f);
					ImGui::ColorEdit4("Left Eye Color", &pigeon.leftEye.color.r);
				}

				if (ImGui::CollapsingHeader("Right Eye"))
				{
					ImGui::DragFloat3("Right Eye", &pigeon.rightEye.center.x, 0.1f);
					ImGui::DragFloat("Right Eye Rx", &pigeon.rightEye.rx, 0.1f);
					ImGui::DragFloat("Right Eye Ry", &pigeon.rightEye.ry, 0.1f);
					ImGui::DragFloat("Right Eye Rz", &pigeon.rightEye.rz, 0.1f);
					ImGui::ColorEdit4("Right Eye Color", &pigeon.rightEye.color.r);
				}
			}

			if (ImGui::CollapsingHeader("Pupils"))
			{
				if (ImGui::CollapsingHeader("Left Pupil"))
				{
					ImGui::DragFloat3("Left Pupil", &pigeon.leftPupil.center.x, 0.1f);
					ImGui::DragFloat("Left Pupil Rx", &pigeon.leftPupil.rx, 0.1f);
					ImGui::DragFloat("Left Pupil Ry", &pigeon.leftPupil.ry, 0.1f);
					ImGui::DragFloat("Left Pupil Rz", &pigeon.leftPupil.rz, 0.1f);
					ImGui::ColorEdit4("Left Pupil Color", &pigeon.leftPupil.color.r);
				}

				if (ImGui::CollapsingHeader("Right Pupil"))
				{
					ImGui::DragFloat3("Right Pupil", &pigeon.rightPupil.center.x, 0.1f);
					ImGui::DragFloat("Right Pupil Rx", &pigeon.rightPupil.rx, 0.1f);
					ImGui::DragFloat("Right Pupil Ry", &pigeon.rightPupil.ry, 0.1f);
					ImGui::DragFloat("Right Pupil Rz", &pigeon.rightPupil.rz, 0.1f);
					ImGui::ColorEdit4("Right Pupil Color", &pigeon.rightPupil.color.r);
				}
			}

			if (ImGui::CollapsingHeader("Mouth"))
			{
				if (!pigeon.cones.empty())
				{
					auto& mouth = pigeon.cones[0]; // Assuming only one mouth for simplicity
					ImGui::DragFloat3("Mouth Center", &mouth.circleCenter.x, 0.1f);
					ImGui::DragFloat3("Mouth Tip", &mouth.coneTip.x, 0.1f);
					ImGui::DragFloat("Mouth Radius", &mouth.radius, 0.1f);
					ImGui::DragInt("Mouth Slices", &mouth.slices);
					ImGui::ColorEdit4("Mouth Color", &pigeon.coneColor.r);
				}
			}
		}
		// Body
		
	}

	else if (mDebugDrawType == DebugDrawType::Oval)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat("Rx", &rx);
		ImGui::DragFloat("Ry", &ry);
		ImGui::DragFloat("Rz", &rz);
		ImGui::DragInt("Slices", &slices);
		ImGui::DragInt("Rings", &rings);
	}

	else if (mDebugDrawType == DebugDrawType::OvalFilled)
	{
		ImGui::DragFloat3("Circle Center", &minExtents.x);
		ImGui::DragFloat("Rx", &rx);
		ImGui::DragFloat("Ry", &ry);
		ImGui::DragFloat("Rz", &rz);
		ImGui::DragInt("Slices", &slices);
		ImGui::DragInt("Rings", &rings);
	}

	ImGui::ColorEdit4("Color", &lineColor.r);
	ImGui::End();
}

