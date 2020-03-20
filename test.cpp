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

#include <thread>
#include <cstdio>
#include <unistd.h>

#include "SimplePA.hpp"

using namespace YukiWorkshop;

int main() {
	// A simple loopback. Turn down your volume first!

	SimplePA::Player p;
	p.open();

	SimplePA::Recorder r;
	r.open();

	std::thread t([&]() {
		puts("Audio thread started.");
		r.run([&p](SimplePA::Recorder &r, const std::vector<uint8_t> &buf) {
			p.play(buf);
			return true;
		}, 1024);
	});

	size_t cnt = 0;
	while (true) {
		printf("Doing some other work... %zu\n", cnt);
		sleep(2);
		cnt++;
	}

}