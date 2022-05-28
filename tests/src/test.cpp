#include <boost/ut.hpp>
#include <pqrs/cf/cf_ptr.hpp>

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "cf_ptr"_test = [] {
    auto cfstring1 = CFStringCreateWithCString(kCFAllocatorDefault, "cfstring1", kCFStringEncodingUTF8);
    auto cfstring2 = CFStringCreateWithCString(kCFAllocatorDefault, "cfstring2", kCFStringEncodingUTF8);
    expect(CFGetRetainCount(cfstring1) == 1);
    expect(CFGetRetainCount(cfstring2) == 1);

    {
      pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
      expect(CFGetRetainCount(cfstring1) == 2);
      expect(ptr1 == true);

      expect(CFGetRetainCount(ptr1.get()) == 2);
      expect(CFGetRetainCount(*ptr1) == 2);

      ptr1.reset();
      expect(CFGetRetainCount(cfstring1) == 1);
      expect(ptr1 == false);
    }

    expect(CFGetRetainCount(cfstring1) == 1);

    {
      pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
      expect(CFGetRetainCount(cfstring1) == 2);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cfstring1) == 1);
    }

    {
      pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
      expect(CFGetRetainCount(cfstring1) == 2);
      pqrs::cf::cf_ptr<CFStringRef> ptr2(cfstring2);
      expect(CFGetRetainCount(cfstring2) == 2);

      ptr1 = ptr2;
      expect(CFGetRetainCount(cfstring1) == 1);
      expect(CFGetRetainCount(cfstring2) == 3);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cfstring1) == 1);
      expect(CFGetRetainCount(cfstring2) == 2);

      ptr2 = nullptr;
      expect(CFGetRetainCount(cfstring1) == 1);
      expect(CFGetRetainCount(cfstring2) == 1);
    }

    {
      pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
      expect(CFGetRetainCount(cfstring1) == 2);

      pqrs::cf::cf_ptr<CFStringRef> ptr1_1(ptr1);
      expect(CFGetRetainCount(cfstring1) == 3);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cfstring1) == 2);

      ptr1_1 = nullptr;
      expect(CFGetRetainCount(cfstring1) == 1);
    }
  };

  return 0;
}
