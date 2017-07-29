#pragma once

#include "ComponentBase.hpp"
#include "CameraSystem.hpp"

namespace Poly {

	class ENGINE_DLLEXPORT PostprocessSettingsComponent : public ComponentBase
	{
		friend void CameraSystem::CameraUpdatePhase(World*);
	public:

		bool UseBgShader = true;
		bool UseFgShader = true;

		int UseCashetes = 0;
		float DistortionPower = 0.45f;
		float ColorTempValue = 7500.0f; // 6500.0 from [1000.0, 40000.0]
		float ColorTempPower = 1.0f;
		float ColorTempLuminancePreservation = 0.75f;
		float SaturationPower = 1.0f;
		float GrainPower = 0.1f;
		float StripesPower = 0.25f;
		float VinettePower = 0.30f;

	};
}