#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace SumEngine;
using namespace SumEngine::Core;
using namespace SumEngine::Graphics;

void App::Run(const AppConfig& config)
{
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appname,
		config.winWidth,
		config.winHeight
	);
	ASSERT(myWindow.IsActive(), "App: failed to create a window");

	// init singletons
	auto handle = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handle, false);
	

	// start state
	ASSERT(mCurrentState != nullptr, "App: no current state available");
	mCurrentState->Initialize();

	// run program
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		if (!myWindow.IsActive())
		{
			Quit();
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		float deltaTime = TimeUtil::GetDeltaTime();

#ifdef _DEBUG
		if (deltaTime < 0.5f)
#endif
		{
			mCurrentState->Update(deltaTime);
		}

		// rendering
	}
	// end state
	mCurrentState->Terminate();

	// terminate singletons
	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}
