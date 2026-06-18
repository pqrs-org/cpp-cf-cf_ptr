#include <iostream>
#include <pqrs/cf/cf_ptr.hpp>

int main() {
  {
    pqrs::cf::cf_ptr<CFDictionaryRef> p;

    if (auto dictionary = CFDictionaryCreate(kCFAllocatorDefault,
                                             nullptr,
                                             nullptr,
                                             0,
                                             &kCFTypeDictionaryKeyCallBacks,
                                             &kCFTypeDictionaryValueCallBacks)) {
      std::cout << CFGetRetainCount(dictionary) << std::endl; // retain count == 1

      p = pqrs::cf::adopt_cf_ptr(dictionary);
      std::cout << CFGetRetainCount(*p) << std::endl; // retain count == 1
    }

    if (p) {
      std::cout << CFDictionaryGetCount(*p) << std::endl;
      std::cout << CFGetRetainCount(*p) << std::endl; // retain count == 1
    }
  }

  return 0;
}
