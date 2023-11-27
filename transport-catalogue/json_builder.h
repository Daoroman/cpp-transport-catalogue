#pragma once

#include"json.h"
#include<optional>

namespace json {



    class Builder {
        class DictItemContext;
        class DictKeyContext;
        class ArrayContext;

    public:
        Builder();
        DictKeyContext Key(std::string key);
        Builder& Value(Node value);
        DictItemContext StartDict();
        ArrayContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
        Node Build();

    private:
        Node root_{ nullptr };
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> key_{ std::nullopt };

        Node GetNode(Node value);
        void StartValue(Node head);

    };

    class Builder::DictItemContext {
    public:
        DictItemContext(Builder& builder);
        DictKeyContext Key(std::string key);
        Builder& EndDict();

    private:
        Builder& builder_;

    };

    class Builder::DictKeyContext {
    public:
        DictKeyContext(Builder& builder);
        DictItemContext Value(Node value);
        ArrayContext StartArray();
        DictItemContext StartDict();

    private:
        Builder& builder_;

    };

    class  Builder::ArrayContext {
    public:
        ArrayContext(Builder& builder);

        ArrayContext Value(Node value);
        DictItemContext StartDict();
        Builder& EndArray();
        ArrayContext StartArray();

    private:
        Builder& builder_;
    };

}