#include <cdxr_incl.h>

#include <iostream>

int main() {

    cdxr::CDXRenderer engine;
	engine.init(nullptr);

    auto info = engine.getVendorsInfo();

	for (const auto& desc : info) {
		std::wcout << L"Vendor Name: " << desc.description << '\n';

		switch (desc.vendorId)
		{
		case 0x10DE: std::println("NVIDIA GPU"); break;
		case 0x1002: std::println("AMD GPU"); break;
		case 0x8086: std::println("Intel GPU"); break;
		}

		std::println("VRAM: {}", desc.dedicatedVideoMemory);
	}

    return 0;
}
