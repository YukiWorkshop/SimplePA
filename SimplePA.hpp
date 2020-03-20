/*
    This file is part of SimplePA.
    Copyright (C) 2020 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <system_error>
#include <vector>
#include <functional>

#include <pulse/simple.h>
#include <pulse/error.h>

#define __PA_ERROR(err)		std::runtime_error(pa_strerror(err))

namespace YukiWorkshop::SimplePA {
	class SampleSpec : public pa_sample_spec {
	public:
		SampleSpec(pa_sample_format_t __format = PA_SAMPLE_S16LE, uint32_t __rate = 44100, uint8_t __channels = 2) {
			format = __format;
			rate = __rate;
			channels = __channels;
		}
	};

	// TODO
	class ChannelMap : public pa_channel_map {

	};

	// TODO
	class BufferAttr : public pa_buffer_attr {

	};

	class Common {
	protected:
		pa_simple *pa_ctx = nullptr;

		SampleSpec sample_spec_;
		ChannelMap channel_map_;
		BufferAttr buffer_attr_;
		bool has_channel_map = false, has_buffer_attr = false;
		std::string device_, server_, name_, stream_name_;

		void __open(pa_stream_direction_t __d);

	public:
		virtual ~Common() {
			if (pa_ctx)
				pa_simple_free(pa_ctx);
		}

		Common& set_name(const std::string& __name) {
			name_ = __name;
			return *this;
		}

		Common& set_stream_name(const std::string& __stream_name) {
			stream_name_ = __stream_name;
			return *this;
		}

		Common& set_device(const std::string& __dev) {
			device_ = __dev;
			return *this;
		}

		Common& set_server(const std::string& __srv) {
			server_ = __srv;
			return *this;
		}

		Common& set_sample_spec(const SampleSpec& __sample_spec) {
			sample_spec_ = __sample_spec;
			return *this;
		}

		Common& set_channel_map(const ChannelMap& __chan_map) {
			channel_map_ = __chan_map;
			return *this;
		}

		Common& set_buffer_attr(const BufferAttr& __buf_attr) {
			buffer_attr_ = __buf_attr;
			return *this;
		}

		const std::string& name() const noexcept {
			return name_;
		}

		const std::string& stream_name() const noexcept {
			return stream_name_;
		}

		const std::string& device() const noexcept {
			return device_;
		}

		const std::string& server() const noexcept {
			return server_;
		}

		const SampleSpec& sample_spec() const noexcept {
			return sample_spec_;
		}

		const ChannelMap* channel_map() const noexcept {
			return has_channel_map ? &channel_map_ : nullptr;
		}

		const BufferAttr* buffer_attr() const noexcept {
			return has_buffer_attr ? &buffer_attr_ : nullptr;
		}

		pa_usec_t latency();
	};

	class Player : public Common {
	public:
		Player(const std::string& __name = "SimplePA",
		       const std::string& __stream_name = "SimplePA Stream",
		       const SampleSpec& __sample_spec = {},
		       const std::string& __device = "",
		       const std::string& __server = "");

		void open() {
			__open(PA_STREAM_PLAYBACK);
		}

		void play(const void *__data, size_t __len);

		template <typename T>
		void play(const std::vector<T>& __data) {
			play(__data.data(), __data.size() * sizeof(T));
		}

		template <typename T>
		void play(const T& __data) {
			play(__data.data(), __data.size());
		}

		void drain();

		void run(const std::function<bool(Player&, std::vector<uint8_t>&)>& __callback);
	};

	class Recorder : public Common {
	public:
		Recorder(const std::string& __name = "SimplePA",
			 const std::string& __stream_name = "SimplePA Stream",
			 const SampleSpec& __sample_spec = {},
			 const std::string& __device = "",
			 const std::string& __server = "");

		void open() {
			__open(PA_STREAM_RECORD);
		}

		void record(void *__data, size_t __len);

		std::vector<uint8_t> record(size_t __len);

		void run(const std::function<bool(Recorder&, const std::vector<uint8_t>&)>& __callback, size_t __buf_size);

	};
}