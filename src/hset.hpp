
#pragma once

#include <memory>
#include <string>

#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include <uv.h>

#include <qdb/hset.h>

#include "entity.hpp"

namespace qdb
{

    class Set : public Entity<Set>
    {

        friend class Entity<Set>;
        friend class Cluster;

    private:
        Set(qdb_handle_t h, const char * alias) : Entity<Set>(h, alias) {}
        virtual ~Set(void) {}

    public:
        static void Init(v8::Handle<v8::Object> exports)
        {
            Entity<Set>::Init(exports, "Set", [](v8::Local<v8::FunctionTemplate> tpl)
            {
                NODE_SET_PROTOTYPE_METHOD(tpl, "insert", insert);
                NODE_SET_PROTOTYPE_METHOD(tpl, "erase", erase);
                NODE_SET_PROTOTYPE_METHOD(tpl, "contains", contains);
            });
        }

    public:
        template <typename F>
        static uv_work_t * spawnRequest(const MethodMan & call, F f)
        {
            Set * pthis = call.nativeHolder<Set>();
            assert(pthis);

            // first position must be the object to process
            ArgsEater eater(call);

            auto buf = eater.eatObject();

            auto callback = eater.eatCallback();
            if (!callback.second)
            {
                call.throwException("callback expected");
                return nullptr;             
            }

            return pthis->MakeWorkItem(callback.first, buf, 0u, f);
        }

    public:
        static void processResult(uv_work_t * req, int status)
        {
            Entity::processVoidResult(req, status);
        }

    public:
        static void insert(const v8::FunctionCallbackInfo<v8::Value> & args)
        {
            Entity::queue_work(args, [](qdb_request * qdb_req)
            {
                qdb_req->output.error = qdb_hset_insert(qdb_req->handle, qdb_req->input.alias.c_str(), qdb_req->input.content.buffer.begin, qdb_req->input.content.buffer.size);
            });
        }

        static void erase(const v8::FunctionCallbackInfo<v8::Value> & args)
        {
            Entity::queue_work(args, [](qdb_request * qdb_req)
            {
                qdb_req->output.error = qdb_hset_erase(qdb_req->handle, qdb_req->input.alias.c_str(), qdb_req->input.content.buffer.begin, qdb_req->input.content.buffer.size);
            });
        }

        static void contains(const v8::FunctionCallbackInfo<v8::Value> & args)
        {
            Entity::queue_work(args, [](qdb_request * qdb_req)
            {
                qdb_req->output.error = qdb_hset_contains(qdb_req->handle, qdb_req->input.alias.c_str(), qdb_req->input.content.buffer.begin, qdb_req->input.content.buffer.size);
            });
        }

    private:
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    private:
        static v8::Persistent<v8::Function> constructor;

    };

}