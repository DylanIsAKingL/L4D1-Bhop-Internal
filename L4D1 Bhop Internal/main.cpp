#include <Windows.h>
#include <thread>
#include <iostream>

namespace offset_1
{
	constexpr std::ptrdiff_t dwForceJump = 0x56ED58; // 0x757DF0 on L4D2 build
	constexpr std::ptrdiff_t onGround = 0x5917C4; // 0x77FBB4 on L4D2 build
}

void bhop()
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	while (true)
	{
		if (!GetAsyncKeyState(VK_SPACE))
			continue;

		const auto onGround = reinterpret_cast<std::int32_t*>(client + offset_1::onGround);

		if (*onGround == 0)
			continue;

		const auto dwForceJump = reinterpret_cast<std::int32_t*>(client + offset_1::dwForceJump);

		if (!dwForceJump)
			continue;

		*dwForceJump = 5;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		*dwForceJump = 4;
	}
}

int __stdcall DllMain(
	HMODULE instance,
	DWORD reason,
	LPVOID reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)bhop, 0, 0, nullptr);

	return 1;
}