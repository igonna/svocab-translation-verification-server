#include <drogon/drogon.h>
#include "icu_check_language.hpp"

using namespace drogon;

int main() {
    app().registerHandler("/is_valid_phrase_langauge", [](const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)>&& callback) {
            auto langCode = req->getHeader("langCode");

            if (langCode.empty() || langCode.size() != 2) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k400BadRequest);
                resp->setBody("Missing langCode header");
                callback(resp);
                return;
            }

            auto body = req->getBody();
            if (body.empty()) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k400BadRequest);
                resp->setBody("Empty body");
                callback(resp);
                return;
            }

            auto phrase = std::string(body);

            if (!is_phrase_language_valid(phrase, langCode.c_str())) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k422UnprocessableEntity);
                callback(resp);
                return;
            }

            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k200OK);
            callback(resp);
        },
        {Post}
    );

    app().addListener("0.0.0.0", 48499).setLogLevel(trantor::Logger::LogLevel::kDebug).run();
    return 0;
}