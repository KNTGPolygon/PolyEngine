#pragma once

#include <Core.hpp>
#include <Engine.hpp>

class GAME_DLLEXPORT TestGame : public Poly::IGame {
public:
	void RegisterEngine(Poly::Engine* engine) override { Engine = engine; }
	void Init() override {};

private:
	Poly::Engine* Engine;
};