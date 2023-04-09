#ifndef AUDIO_STREAM_FLAC_H
#define AUDIO_STREAM_FLAC_H

#include "core/io/resource.h"
#include "core/io/resource_loader.h"
#include "core/object/ref_counted.h"
#include "servers/audio/audio_stream.h"
#include "thirdparty/dr_flac/dr_flac.h"

class AudioStreamFLAC : public AudioStream {
	GDCLASS(AudioStreamFLAC, AudioStream);
	OBJ_SAVE_TYPE(AudioStream) //children are all saved as AudioStream, so they can be exchanged
	RES_BASE_EXTENSION("flacstr");

	friend class AudioStreamPlaybackFLAC;

	void *data;
	uint32_t data_len;

	float sample_rate;
	int channels;
	float length;
	bool loop;
	float loop_offset;
	void clear_data();

	double bpm = 0;
	int beat_count = 0;
	int bar_beats = 4;

protected:
	static void _bind_methods();

public:
	void set_loop(bool p_enable);
	virtual bool has_loop() const override;

	void set_loop_offset(double p_seconds);
	double get_loop_offset() const;

	void set_bpm(double p_bpm);
	virtual double get_bpm() const override;

	void set_beat_count(int p_beat_count);
	virtual int get_beat_count() const override;

	void set_bar_beats(int p_bar_beats);
	virtual int get_bar_beats() const override;

	virtual Ref<AudioStreamPlayback> instantiate_playback();
	virtual String get_stream_name() const;

	void set_data(const Vector<uint8_t> &p_data);
	Vector<uint8_t> get_data() const;

	virtual double get_length() const; //if supported, otherwise return 0

	virtual bool is_monophonic() const override;

	AudioStreamFLAC();
	virtual ~AudioStreamFLAC();
};

class AudioStreamPlaybackFLAC : public AudioStreamPlaybackResampled {
	GDCLASS(AudioStreamPlaybackFLAC, AudioStreamPlaybackResampled)

	enum {
		FADE_SIZE = 256
	};
	AudioFrame loop_fade[FADE_SIZE];
	int loop_fade_remaining = FADE_SIZE;

	drflac *pFlac;
	uint32_t frames_mixed = 0;
	bool active = false;
	int loops = 0;

	friend class AudioStreamFLAC;

	Ref<AudioStreamFLAC> flac_stream;

protected:
	virtual int _mix_internal(AudioFrame *p_buffer, int p_frames) override;
	virtual float get_stream_sampling_rate() override;

public:
	virtual void start(double p_from_pos = 0.0);
	virtual void stop();
	virtual bool is_playing() const;

	virtual int get_loop_count() const; // times it looped

	virtual double get_playback_position() const;
	virtual void seek(double p_time) override;

	//virtual int mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);
	//virtual float get_length() const; // if supported, otherwise return 0
	AudioStreamPlaybackFLAC();
	~AudioStreamPlaybackFLAC();
};

#endif