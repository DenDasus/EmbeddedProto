{% macro enum_macro(_enum) %}
enum {{ _enum.name }}
{
  {% for value in _enum.values() %}
  {{ value.name }} = {{ value.number }}{{ "," if not loop.last }}
  {% endfor %}
};

{% endmacro %}
{# #}
{# ------------------------------------------------------------------------------------------------------------------ #}
{# #}
{% macro field_get_set_macro(_field) %}
static constexpr uint32_t {{_field.variable_id_name}} = {{_field.variable_id}};
{% if _field.is_repeated_field %}
inline const {{_field.type}}& {{_field.name}}(uint32_t index) const { return {{_field.variable_name}}[index]; }
inline void clear_{{_field.name}}() { {{_field.variable_name}}.clear(); }
{% if _field.which_oneof is defined %}
inline void set_{{_field.name}}(uint32_t index, const {{_field.type}}& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}}.set(index, value);
}
inline void set_{{_field.name}}(uint32_t index, const {{_field.type}}&& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}}.set(index, value);
}
inline void add_{{_field.name}}(const {{_field.type}}& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}}.add(value);
}
inline {{_field.repeated_type}}& mutable_{{_field.name}}()
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  return {{_field.variable_name}};
}
{% else %}
inline void set_{{_field.name}}(uint32_t index, const {{_field.type}}& value) { {{_field.variable_name}}.set(index, value); }
inline void set_{{_field.name}}(uint32_t index, const {{_field.type}}&& value) { {{_field.variable_name}}.set(index, value); }
inline void add_{{_field.name}}(const {{_field.type}}& value) { {{_field.variable_name}}.add(value); }
inline {{_field.repeated_type}}& mutable_{{_field.name}}() { return {{_field.variable_name}}; }
{% endif %}
inline const {{_field.repeated_type}}& get_{{_field.name}}() const { return {{_field.variable_name}}; }
{% elif _field.of_type_message %}
inline const {{_field.type}}& {{_field.name}}() const { return {{_field.variable_name}}; }
inline void clear_{{_field.name}}() { {{_field.variable_name}}.clear(); }
{% if _field.which_oneof is defined %}
inline void set_{{_field.name}}(const {{_field.type}}& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}} = value;
}
inline void set_{{_field.name}}(const {{_field.type}}&& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}} = value;
}
inline {{_field.type}}& mutable_{{_field.name}}()
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  return {{_field.variable_name}};
}
{% else %}
inline void set_{{_field.name}}(const {{_field.type}}& value) { {{_field.variable_name}} = value; }
inline void set_{{_field.name}}(const {{_field.type}}&& value) { {{_field.variable_name}} = value; }
inline {{_field.type}}& mutable_{{_field.name}}() { return {{_field.variable_name}}; }
{% endif %}
inline const {{_field.type}}& get_{{_field.name}}() const { return {{_field.variable_name}}; }
{% elif _field.of_type_enum %}
inline {{_field.type}} {{_field.name}}() const { return {{_field.variable_name}}; }
inline void clear_{{_field.name}}() { {{_field.variable_name}} = static_cast<{{_field.type}}>({{_field.default_value}}); }
{% if _field.which_oneof is defined %}
inline void set_{{_field.name}}(const {{_field.type}}& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}} = value;
}
inline void set_{{_field.name}}(const {{_field.type}}&& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}} = value;
}
{% else %}
inline void set_{{_field.name}}(const {{_field.type}}& value) { {{_field.variable_name}} = value; }
inline void set_{{_field.name}}(const {{_field.type}}&& value) { {{_field.variable_name}} = value; }
{% endif %}    inline {{_field.type}} get_{{_field.name}}() const { return {{_field.variable_name}}; }
{% else %}
inline {{_field.type}}::FIELD_TYPE {{_field.name}}() const { return {{_field.variable_name}}.get(); }
inline void clear_{{_field.name}}() { {{_field.variable_name}}.set({{_field.default_value}}); }
{% if _field.which_oneof is defined %}
inline void set_{{_field.name}}(const {{_field.type}}::FIELD_TYPE& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}}.set(value);
}
inline void set_{{_field.name}}(const {{_field.type}}::FIELD_TYPE&& value)
{
  {{_field.which_oneof}} = {{_field.variable_id}};
  {{_field.variable_name}}.set(value);
}
{% else %}
inline void set_{{_field.name}}(const {{_field.type}}::FIELD_TYPE& value) { {{_field.variable_name}}.set(value); }
inline void set_{{_field.name}}(const {{_field.type}}::FIELD_TYPE&& value) { {{_field.variable_name}}.set(value); }
{% endif %}
inline {{_field.type}}::FIELD_TYPE get_{{_field.name}}() const { return {{_field.variable_name}}.get(); }
{% endif %}
{% endmacro %}
{# #}
{# ------------------------------------------------------------------------------------------------------------------ #}
{# #}
{% macro field_serialize_macro(_field) %}
{% if _field.is_repeated_field %}
if(result)
{
  result = {{_field.variable_name}}.serialize({{_field.variable_id_name}}, buffer);
}
{% elif _field.of_type_message %}
if(result)
{
  const ::EmbeddedProto::MessageInterface* x = &{{_field.variable_name}};
  result = x->serialize({{_field.variable_id_name}}, buffer);
}
{% elif _field.of_type_enum %}
if(({{_field.default_value}} != {{_field.variable_name}}) && result)
{
  EmbeddedProto::uint32 value;
  value.set(static_cast<uint32_t>({{_field.variable_name}}));
  result = value.serialize({{_field.variable_id_name}}, buffer);
}
{% else %}
if(({{_field.default_value}} != {{_field.variable_name}}.get()) && result)
{
  result = {{_field.variable_name}}.serialize({{_field.variable_id_name}}, buffer);
} {% endif %} {% endmacro %}
{# #}
{# ------------------------------------------------------------------------------------------------------------------ #}
{# #}
{% macro field_deserialize_macro(_field) %}
{% if _field.is_repeated_field %}
if(::EmbeddedProto::WireFormatter::WireType::LENGTH_DELIMITED == wire_type)
{
  result = {{_field.variable_name}}.deserialize(buffer);
}
{% else %}
if(::EmbeddedProto::WireFormatter::WireType::{{_field.wire_type}} == wire_type)
{
  {% if _field.of_type_message %}
  uint32_t size;
  result = ::EmbeddedProto::WireFormatter::DeserializeVarint(buffer, size);
  ::EmbeddedProto::ReadBufferSection bufferSection(buffer, size);
  result = result && {{_field.variable_name}}.deserialize(bufferSection);
  {% elif _field.of_type_enum %}
  uint32_t value;
  result = ::EmbeddedProto::WireFormatter::DeserializeVarint(buffer, value);
  if(result)
  {
    {{_field.variable_name}} = static_cast<{{_field.type}}>(value);
    {% if _field.which_oneof is defined %}
    {{_field.which_oneof}} = {{_field.variable_id}};
    {% endif %}
  }
  {% else %}
  result = {{_field.variable_name}}.deserialize(buffer);
  {% if _field.which_oneof is defined %}
  if(result)
  {
    {{_field.which_oneof}} = {{_field.variable_id}};
  }
  {% endif %}
  {% endif %}
}
{% endif %}
else
{
  // TODO Error wire type does not match field.
  result = false;
} {% endmacro %}
{# #}
{# ------------------------------------------------------------------------------------------------------------------ #}
{# #}
{% macro msg_macro(msg) %}
{% if msg.templates is defined %}
{% for template in msg.templates %}
{{"template<" if loop.first}}uint32_t {{template}}{{"SIZE, " if not loop.last}}{{"SIZE>" if loop.last}}
{% endfor %}
{% endif %}
class {{ msg.name }} final: public ::EmbeddedProto::MessageInterface
{
  public:
    {{ msg.name }}() :
    {% for field in msg.fields() %}
        {% if field.of_type_enum %}
        {{field.variable_name}}({{field.default_value}}){{"," if not loop.last}}
        {% else %}
        {{field.variable_name}}(){{"," if not loop.last}}
        {% endif %}
    {% endfor %}
    {

    };
    ~{{ msg.name }}() override = default;

    {% for enum in msg.nested_enums() %}
    {{ enum_macro(enum) }}

    {% endfor %}
    {% for field in msg.fields() %}
    {{ field_get_set_macro(field)|indent(4) }}
    {% endfor %}
    {% for oneof in msg.oneofs() %}
    {% for field in oneof.fields() %}
    {{ field_get_set_macro(field)|indent(4) }}
    {% endfor %}
    {% endfor %}
    bool serialize(::EmbeddedProto::WriteBufferInterface& buffer) const final
    {
      bool result = true;

      {% for field in msg.fields() %}
      {{ field_serialize_macro(field)|indent(6) }}

      {% endfor %}
      {% for oneof in msg.oneofs() %}
      if((0 != {{oneof.which_oneof}}) && result)
      {
        switch({{oneof.which_oneof}})
        {
          {% for field in oneof.fields() %}
          case {{field.variable_id_name}}:
            {{ field_serialize_macro(field)|indent(12) }}
            break;

          {% endfor %}
          default:
            break;
        }
      }

      {% endfor %}
      return result;
    };

    bool deserialize(::EmbeddedProto::ReadBufferInterface& buffer) final
    {
      bool result = true;
      ::EmbeddedProto::WireFormatter::WireType wire_type;
      uint32_t id_number = 0;

      while(result && ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number))
      {
        switch(id_number)
        {
          {% for field in msg.fields() %}
          case {{field.variable_id_name}}:
          {
            {{ field_deserialize_macro(field)|indent(12) }}
            break;
          }

          {% endfor %}
          {% for oneof in msg.oneofs() %}
          {% for field in oneof.fields() %}
          case {{field.variable_id_name}}:
          {
            {{ field_deserialize_macro(field)|indent(12) }}
            break;
          }

          {% endfor %}
          {% endfor %}
          default:
            break;
        }
      }
      return result;
    };

    void clear() final
    {
      {% for field in msg.fields() %}
      clear_{{field.name}}();
      {% endfor %}
    }

  private:

    {% for field in msg.fields() %}
    {% if field.is_repeated_field %}
    {{field.repeated_type}} {{field.variable_name}};
    {% else %}
    {{field.type}} {{field.variable_name}};
    {% endif %}
    {% endfor %}

    {% for oneof in msg.oneofs() %}
    ::EmbeddedProto::uint32 {{oneof.which_oneof}};
    {% for field in oneof.fields() %}
    {% if field.is_repeated_field %}
    {{field.repeated_type}} {{field.variable_name}};
    {% else %}
    {{field.type}} {{field.variable_name}};
    {% endif %}
    {% endfor %}
    {% endfor %}
};
{% endmacro %}
{# #}
{# ------------------------------------------------------------------------------------------------------------------ #}
{# #}
// This file is generated. Please do not edit!
#ifndef _{{filename.upper()}}_H_
#define _{{filename.upper()}}_H_

#include <cstdint>
{% if messages %}
#include <MessageInterface.h>
#include <WireFormatter.h>
#include <Fields.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedField.h>
{% endif %}

{% if namespace %}
namespace {{ namespace }}
{ {% endif %}

{% for enum in enums %}
{{ enum_macro(enum) }}
{% endfor %}
{% for msg in messages %}
{{ msg_macro(msg) }}
{% endfor %}
{% if namespace %}
} // End of namespace {{ namespace }}
{% endif %}
#endif // _{{filename.upper()}}_H_

