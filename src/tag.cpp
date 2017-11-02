#include <wxx/tag.h>

using namespace wxx;


bool tag::is_dynamic() const
{
	for (const auto& v: attrs_) {
		if (!v.v_.static_)
			return true;
	}

	return !cont_.static_;
}

js_stream& js_stream::operator<<(const table_element& elm)
{
	assert(elm.t_.name_ == "tr" || elm.t_.name_ == "td");

	const tag& t = elm.t_;

	if (t.name_ == "tr") {
		s_ 
			<< "var tc=function() { var p = table.insertRow(" << elm.ind_ << "); var row = p;" << std::endl;
	}
	else if (t.name_ == "td") {
		s_
			<< "var tc=function() { var p = row.insertCell("<< elm.ind_ << ");" << std::endl;
	}

	//create attributes
	for( const auto& v: t.attrs_) {
		var<tag_t> p{"p"};
		s_ << set_attribute(p, v) << ";";
	}

	//create kids
	for (const auto& v: t.tags_) {
		*this << v;
		s_ << "var tag = tc(); p.appendChild(tag);" << std::endl;
	}

	if (!t.cont_.empty()) {
		s_ << "p.appendChild(document.createTextNode(" << t.cont_ << "));" << std::endl;
	}


	s_ << "return p; }" << std::endl;

	return *this;
}

js_stream& js_stream::operator<<( const tag& t )
{
	assert(t.name_ != "tr" && t.name_ != "td");

	if (t.name_ == "table") {
		assert(!table_row_ && !table_cell_);  //nested tables aren't supported
		s_ 
			<< "var tc=function() { var p = document.createElement(\""
			<< t.name_
			<< "\"); var table = p;" << std::endl;
	}
	else {
		s_ 
			<< "var tc=function() { var p = document.createElement(\""
			<< t.name_
			<< "\");" << std::endl;
	}

	//create attributes
	for( const auto& v: t.attrs_) {
		var<tag_t> p{"p"};
		s_ << set_attribute(p, v) << ";";
	}

	//create kids
	for (const auto& v: t.tags_) {
		*this << v;
		s_ << "var tag = tc(); p.appendChild(tag);" << std::endl;
	}

	if (!t.cont_.empty()) {
		s_ << "p.appendChild(document.createTextNode(" << t.cont_ << "));" << std::endl;
	}

	s_ << "return p; }" << std::endl;
	return *this;
}


