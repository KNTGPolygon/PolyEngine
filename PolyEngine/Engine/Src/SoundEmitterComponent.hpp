#pragma once

#include "ComponentBase.hpp"

namespace Poly
{
	class SoundResource;

	/// Class representing sound source.
	/// Sound source is located where enititys TransformComponent points.
	/// Emitters by default have enabled Doppler effect and default distance model.
	/// If you wish you can change default distance model on your own or particularly for one of emitters.
	/// @see SoundListenerComponent
	/// @see SoundSystem 
	class ENGINE_DLLEXPORT SoundEmitterComponent : public ComponentBase
	{
	public:
		SoundEmitterComponent();
		~SoundEmitterComponent();

		void PushSoundResource(SoundResource* val) { Playlist.PushBack(val); PlaylistChanged = true; }
		void PopSoundResource() { Playlist.PopFront; PlaylistChanged = true; }
		const SoundResource* GetBuffer(size_t idx) { return Playlist[idx]; }
		size_t GetBufferCount() { return Playlist.GetSize(); }

		float Pitch = 1.0f;
		float Gain = 1.0f;
		float MaxDistance = 100.0f;
		float RolloffFactor = 1.0f;
		float RefferenceDistance = 100.0f;
		float MinGain = 1.0f;
		float MaxGain = 1.0f;
		float ConeInnerAngle = 360.0f;
		float ConeOuterAngle = 360.0f;
		float ConeOuterGain = 1.0f;
		float SecondsOffset = 0.0f;
		bool Looping = false;
		bool Paused = false;
		bool PlaylistChanged = false;

	private:
		Dynarray<SoundResource*> Playlist;
	};

} // namespace Poly