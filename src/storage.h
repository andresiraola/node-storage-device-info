#ifndef STORAGE_H
#define STORAGE_H

/**
 ** The following warnings are displayed during compilation on win32 platforms
 ** using node-gyp:
 **
 **  - C++ exception handler used, but unwind semantics are not enabled.
 **  - no definition for inline function 'v8::Persistent<T> \
 **       v8::Persistent<T>::New(v8::Handle<T>)'
 **
 ** There don't seem to be any issues which would suggest these are real
 ** problems, so we've disabled them for now.
 **/
#ifdef _WIN32
#pragma warning(disable:4506;disable:4530)
#endif

#include <node.h>

#include <list>
#include <string>

using namespace v8;

namespace storage {

class DeviceInfoWrap : public node::ObjectWrap {
public:
	static void Init(Handle<Object> target);

	static void GetPartitionSpaceRequestBegin(uv_work_t* request);
	static void GetPartitionSpaceRequestEnd(uv_work_t* request, int status);

private:
	DeviceInfoWrap();
	~DeviceInfoWrap();

	static Handle<Value> New(const Arguments& args);

	static Handle<Value> GetPartitionSpace(const Arguments& args);
};

struct GetPartitionSpaceRequest {
	GetPartitionSpaceRequest(const char *path_value) : path(path_value) {}
	~GetPartitionSpaceRequest() {}
	
	uv_work_t uv_request;
	
	Persistent<Function> cb;

	std::string path;
	
#ifdef _WIN32
	DWORD rcode;
#else /* _WIN32 */
	int rcode;
#endif /* _WIN32 */

	unsigned int total;
	unsigned int free;

	DeviceInfoWrap* device_info;
};

}; /* namespace storage */

#endif /* STORAGE_H */
