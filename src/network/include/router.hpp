#ifndef ROUTER_H
#define ROUTER_H

#include <unordered_map>
#include <string_view>
#include <functional>
#include "../../parsing/include/http_types.hpp"

namespace aether {
	typedef std::function<HttpResponse(HttpRequest&)> Callback;

	struct Router {
		void addCallback(
			const std::string_view& path,
			Callback func
		);
		void setDefaultCallback(Callback func);
		Callback getCallback(const std::string_view& path);
	private:
		std::unordered_map<std::string_view, Callback> callbacks_;

		Callback default_callback_ = [](HttpRequest&) -> HttpResponse {
			return HttpResponse(404);
		};
	};
};

#endif // ROUTER_H
