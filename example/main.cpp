#include <iostream>
#include <pqrs/cf/cf_ptr.hpp>

int main(void) {
  {
    pqrs::cf::cf_ptr<CFStringRef> p;

    if (auto string = CFStringCreateWithCString(kCFAllocatorDefault,
                                                "example",
                                                kCFStringEncodingUTF8)) {
      p = string;

      CFRelease(string);
    }

    std::cout << CFGetRetainCount(*p) << std::endl; // CFGetRetainCount(*p) == 1
    std::cout << CFStringGetCStringPtr(*p, kCFStringEncodingUTF8) << std::endl;
  }

  return 0;
}
