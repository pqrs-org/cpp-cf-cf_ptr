#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/cf/cf_ptr.hpp>

TEST_CASE("cf_ptr") {
  auto cfstring1 = CFStringCreateWithCString(kCFAllocatorDefault, "cfstring1", kCFStringEncodingUTF8);
  auto cfstring2 = CFStringCreateWithCString(kCFAllocatorDefault, "cfstring2", kCFStringEncodingUTF8);
  REQUIRE(CFGetRetainCount(cfstring1) == 1);
  REQUIRE(CFGetRetainCount(cfstring2) == 1);

  {
    pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
    REQUIRE(CFGetRetainCount(cfstring1) == 2);
    REQUIRE(ptr1 == true);

    REQUIRE(CFGetRetainCount(ptr1.get()) == 2);
    REQUIRE(CFGetRetainCount(*ptr1) == 2);

    ptr1.reset();
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
    REQUIRE(ptr1 == false);
  }

  REQUIRE(CFGetRetainCount(cfstring1) == 1);

  {
    pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
    REQUIRE(CFGetRetainCount(cfstring1) == 2);

    ptr1 = nullptr;
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
  }

  {
    pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
    REQUIRE(CFGetRetainCount(cfstring1) == 2);
    pqrs::cf::cf_ptr<CFStringRef> ptr2(cfstring2);
    REQUIRE(CFGetRetainCount(cfstring2) == 2);

    ptr1 = ptr2;
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
    REQUIRE(CFGetRetainCount(cfstring2) == 3);

    ptr1 = nullptr;
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
    REQUIRE(CFGetRetainCount(cfstring2) == 2);

    ptr2 = nullptr;
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
    REQUIRE(CFGetRetainCount(cfstring2) == 1);
  }

  {
    pqrs::cf::cf_ptr<CFStringRef> ptr1(cfstring1);
    REQUIRE(CFGetRetainCount(cfstring1) == 2);

    pqrs::cf::cf_ptr<CFStringRef> ptr1_1(ptr1);
    REQUIRE(CFGetRetainCount(cfstring1) == 3);

    ptr1 = nullptr;
    REQUIRE(CFGetRetainCount(cfstring1) == 2);

    ptr1_1 = nullptr;
    REQUIRE(CFGetRetainCount(cfstring1) == 1);
  }
}
