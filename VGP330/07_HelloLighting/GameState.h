#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;

	void Update(float deltaTime);


protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;

	SumEngine::Graphics::RenderObject mRenderObject;
	SumEngine::Graphics::RenderObject mRenderObject2;

	SumEngine::Graphics::StandardEffect mStandardEffect;

};
