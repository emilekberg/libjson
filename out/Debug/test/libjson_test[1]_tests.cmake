add_test([=[LIBJSON.someTestCase]=]  /home/emilekberg/dev/json/out/Debug/test/libjson_test [==[--gtest_filter=LIBJSON.someTestCase]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[LIBJSON.someTestCase]=]  PROPERTIES WORKING_DIRECTORY /home/emilekberg/dev/json/out/Debug/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  libjson_test_TESTS LIBJSON.someTestCase)
