#if !defined(ARROW_TESTSUITE_H_)
#define ARROW_TESTSUITE_H_

#include <arrow/gateway.h>
#include <arrow/device.h>

// start the gateway test
int arrow_test_gateway(arrow_gateway_t *gateway, property_t *res_hid);
int arrow_test_gateway_proc_hid(arrow_gateway_t *gateway, const char *phid, property_t *res_hid);
int arrow_test_gateway_proc_name(arrow_gateway_t *gateway, const char *name, property_t *res_hid);
// start the device test
int arrow_test_device(arrow_device_t *device, property_t *res_hid);
int arrow_test_device_proc_hid(arrow_device_t *device, const char *phid, property_t *res_hid);
int arrow_test_device_proc_name(arrow_device_t *device, const char *name, property_t *res_hid);
// start the test case
int arrow_test_begin(property_t *res_hid);
// try to finish this test case
int arrow_test_end(property_t *res_hid);
// start current test step
int arrow_test_step_begin(property_t *res_hid, int number);
// notify that test step successed
int arrow_test_step_success(property_t *res_hid, int number);
// notify that test step failed
int arrow_test_step_fail(property_t *res_hid, int number, const char *error);
// notify that test step skipped
int arrow_test_step_skip(property_t *res_hid, int number);

#endif  // ARROW_TESTSUITE_H_
