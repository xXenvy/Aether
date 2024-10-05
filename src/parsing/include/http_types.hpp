#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H

#include <string>
#include <unordered_map>

namespace aether {
	enum class Method {
		GET,
		POST,
		PUT,
		PATCH,
		DELETE_,
		HEAD,
		OPTIONS,
	};

	struct HttpRequest {
		std::string path;
		std::string version;
		std::string body;

		Method method;
		std::unordered_map<std::string, std::string> headers;
	};

	struct HttpResponse {
		HttpResponse(
			unsigned int status,
			const std::string& body = "",
			const std::string& contentType = ""
		) : status(status), body(body), contentType(contentType) {};
		unsigned int status;
		const std::string body;
		const std::string contentType;

	};
};

#endif // HTTP_TYPES_H