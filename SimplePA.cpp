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

#include "SimplePA.hpp"

using namespace YukiWorkshop;

void SimplePA::Common::__open(pa_stream_direction_t __d) {
	int err;
	pa_ctx = pa_simple_new(server_.empty() ? nullptr : server_.c_str(),
			       name_.c_str(),
			       __d,
			       device_.empty() ? nullptr : device_.c_str(),
			       stream_name_.c_str(),
			       &sample_spec_,
			       has_channel_map ? &channel_map_ : nullptr,
			       has_buffer_attr ? &buffer_attr_ : nullptr,
			       &err);

	if (!pa_ctx)
		throw __PA_ERROR(err);
}

pa_usec_t SimplePA::Common::latency() {
	int err;
	pa_usec_t ret = pa_simple_get_latency(pa_ctx, &err);

	if (err == (pa_usec_t) -1)
		throw __PA_ERROR(err);

	return ret;
}

SimplePA::Player::Player(const std::string &__name, const std::string &__stream_name,
			 const SimplePA::SampleSpec &__sample_spec, const std::string &__device,
			 const std::string &__server) {
	set_name(__name);
	set_stream_name(__stream_name);
	set_sample_spec(__sample_spec);
	set_device(__device);
	set_server(__server);
}

void SimplePA::Player::play(const void *__data, size_t __len) {
	int err;
	if (pa_simple_write(pa_ctx, __data, __len, &err))
		throw __PA_ERROR(err);
}

void SimplePA::Player::drain() {
	int err;
	if (pa_simple_drain(pa_ctx, &err))
		throw __PA_ERROR(err);
}

void SimplePA::Player::run(const std::function<bool(Player & , std::vector<uint8_t> & )> &__callback) {
	std::vector<uint8_t> buf;

	while (__callback(*this, buf)) {
		play(buf);
	}
}

SimplePA::Recorder::Recorder(const std::string &__name, const std::string &__stream_name,
			     const SimplePA::SampleSpec &__sample_spec, const std::string &__device,
			     const std::string &__server) {
	set_name(__name);
	set_stream_name(__stream_name);
	set_sample_spec(__sample_spec);
	set_device(__device);
	set_server(__server);
}

void SimplePA::Recorder::record(void *__data, size_t __len) {
	int err;
	if (pa_simple_read(pa_ctx, __data, __len, &err))
		throw __PA_ERROR(err);
}

std::vector<uint8_t> SimplePA::Recorder::record(size_t __len) {
	std::vector<uint8_t> ret(__len);
	record(ret.data(), ret.size());
	return ret;
}

void SimplePA::Recorder::run(const std::function<bool(Recorder&, const std::vector<uint8_t>&)> &__callback, size_t __buf_size) {
	std::vector<uint8_t> buf(__buf_size);

	do {
		record(buf.data(), buf.size());
	} while (__callback(*this, buf));
}
