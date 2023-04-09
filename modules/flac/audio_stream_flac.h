#ifndef AUDIO_STREAM_FLAC_H
#define AUDIO_STREAM_FLAC_H

#include "core/io/resource.h"
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

protected:
	static void _bind_methods();

public:
	void set_loop(bool p_enable);
	bool has_loop() const;

	void set_loop_offset(float p_seconds);
	float get_loop_offset() const;

	virtual Ref<AudioStreamPlayback> instantiate_playback();
	virtual String get_stream_name() const;

	void set_data(const Vector<uint8_t> &p_data);
	Vector<uint8_t> get_data() const;

	virtual double get_length() const; //if supported, otherwise return 0

	AudioStreamFLAC();
	virtual ~AudioStreamFLAC();
};

class AudioStreamPlaybackFLAC : public AudioStreamPlaybackResampled {
	GDCLASS(AudioStreamPlaybackFLAC, AudioStreamPlaybackResampled)

	drflac *pFlac;
	uint32_t frames_mixed;

	bool active;
	int loops;

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
	virtual void seek(float p_time);

	//virtual int mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);
	//virtual float get_length() const; // if supported, otherwise return 0
	AudioStreamPlaybackFLAC();
	~AudioStreamPlaybackFLAC();
};

#endif