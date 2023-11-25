#include "json_builder.h"

namespace json {
	Builder::Builder() {
		Node* root_ptr = &root_;
		nodes_stack_.emplace_back(root_ptr);
	}

    Builder::DictKeyContext Builder::Key(std::string key) {
		auto* head = nodes_stack_.back();

		if (head->IsMap () && !key_) key_ = std::move(key);
		else throw std::logic_error("Wrong Key");

		return *this;
	}

	Builder& Builder::Value(Node value) {
        auto* head = nodes_stack_.back();
        if (head->IsMap()) {
            if (!key_) throw std::logic_error("No Key");
            std::map<std::string, Node>& dict = std::get<Dict>(head->GetValue());
            dict.emplace(std::move(key_.value()), std::move((GetNode(value))));
            key_ = std::nullopt;
           head->GetValue() =dict;
        }
        else if (head->IsArray()) {
            auto& array = std::get<Array>(head->GetValue());
            array.emplace_back(GetNode(value));
            head = &array.back();
        }
        else if (root_.IsNull()) {
            root_.GetValue() = value.GetValue();
        }
        else throw std::logic_error("Wrong node");

        return *this;
    }
	
    Builder::DictItemContext Builder::StartDict() {
        auto* head = nodes_stack_.back();

        if (head->IsMap()) {
            if (!key_) throw std::logic_error("No Key for Dict");
           auto& dict = std::get<Dict>(head->GetValue());
           auto ret = dict.emplace(std::move(key_.value()), Dict());
           nodes_stack_.emplace_back(&ret.first->second);
           key_ = std::nullopt;
        }
        else if (head->IsArray()) {
           auto& array = std::get<Array>(head->GetValue());
           array.emplace_back(Dict());
            nodes_stack_.emplace_back(&array.back());
        }
        else if (head->IsNull()) {
            head->GetValue() = Dict();
        }
        else throw std::logic_error("Wrong node");

        return *this;
    }

    Builder::ArrayContext Builder::StartArray() {
        auto* head = nodes_stack_.back();

        if (head->IsMap()) {
            if (!key_) throw std::logic_error("No Key for Dict");
          auto& dict = std::get<Dict>(head->GetValue());
          auto ret = dict.emplace(std::move(key_.value()), Array());
          nodes_stack_.emplace_back(&ret.first->second);
           key_ = std::nullopt;
        }
        else if (head->IsArray()) {
            auto& array = std::get<Array>(head->GetValue());
            array.emplace_back(Array());
            nodes_stack_.emplace_back(&array.back());
        }
        else if (head->IsNull()) {
            head->GetValue() = Array();
        }
        else throw std::logic_error("Wrong  node");

        return *this;
    }

    Builder& Builder::EndDict() {
        auto* head = nodes_stack_.back();

        if (!head->IsMap()) throw std::logic_error("Can not and Dict thar is not Dict");
        nodes_stack_.pop_back();

        return *this;
    }

    Builder& Builder::EndArray() {
        auto* head = nodes_stack_.back();

        if (!head->IsArray()) throw std::logic_error("Can not and Array thar is not Array");
        nodes_stack_.pop_back();

        return *this;
    }

    Node Builder::Build() {
        if (root_.IsNull() || nodes_stack_.size() > 1) throw std::logic_error("Wrong Order");
        return root_;
    }

    Node Builder::GetNode(Node value) {
        if (std::holds_alternative<int>(value.GetValue())) return Node(std::get<int>(value.GetValue()));
        if (std::holds_alternative<double>(value.GetValue())) return Node(std::get<double>(value.GetValue()));
        if (std::holds_alternative<std::string>(value.GetValue())) return Node(std::get<std::string>(value.GetValue()));
        if (std::holds_alternative<std::nullptr_t>(value.GetValue())) return Node(std::get<std::nullptr_t>(value.GetValue()));
        if (std::holds_alternative<bool>(value.GetValue())) return Node(std::get<bool>(value.GetValue()));
        if (std::holds_alternative<Dict>(value.GetValue())) return Node(std::get<Dict>(value.GetValue()));
        if (std::holds_alternative<Array>(value.GetValue())) return Node(std::get<Array>(value.GetValue()));
        return {};
    }

    Builder::DictItemContext::DictItemContext(Builder& builder)
        : builder_(builder)
    {}

    Builder::DictKeyContext  Builder::DictItemContext::Key(std::string key) {
        return builder_.Key(key); 
    }

    Builder& Builder::DictItemContext::EndDict() {
        return builder_.EndDict();
    }

    Builder::DictKeyContext::DictKeyContext(Builder& builder)
        : builder_(builder)
    {}

    Builder::DictItemContext  Builder::DictKeyContext::Value(Node value) {
        return DictItemContext(builder_.Value(value));
    }

    Builder::ArrayContext  Builder::DictKeyContext::StartArray() {
        return builder_.StartArray();
    }

    Builder::DictItemContext  Builder::DictKeyContext::StartDict() {
        return builder_.StartDict();
    }

    Builder::ArrayContext::ArrayContext(Builder& builder)
        : builder_(builder)
    {}

    Builder::ArrayContext  Builder::ArrayContext::Value(Node value) {
        return ArrayContext(builder_.Value(value));
    }

    Builder::DictItemContext  Builder::ArrayContext::StartDict() {
        return builder_.StartDict();
    }

     Builder::ArrayContext  Builder::ArrayContext::StartArray() {
        return builder_.StartArray();
    }

    Builder& Builder::ArrayContext::EndArray() {
        return builder_.EndArray();
    }
}