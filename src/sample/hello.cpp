#include <iostream>
#include <strstream>
#include <wxx/wxx.h>

using namespace wxx;

int main()
{
	page sample;
	sample.add_keywords({"wxx", "sample"});

	//the page body is the 'body' tag. we assign an id to it that will be used to add dynamic tags to body
	sample.body() += {"id", "sample_body"};

	//static div
	{
		tag div{"div"};
		div += "Hello! I am a static div."_ent;
		sample.body() += div; //add to body
	}


	//hybrid div
	{
		// begine javascript module
		std::ostringstream ss;
		auto mod = module_(ss);
		
		auto hybrid_text = mod.make_var<std::string>(); //declare a JS variable in this module
		mod
			[hybrid_text = "Hello! I am a hybrid div."_v]
			;
		sample += script(ss.str()); //add the script to the page

		//now use the JS variable in the tag definition directly
		tag div{"div"};
		div += {"style", "color:blue;font-size:1.5em"};
		div += hybrid_text; //the dynamic portion of the div element

		//add the tag with a dynamic content to the page body
		sample.body() += div;
	}

	//dynamic div
	{
		tag div{"div"};
		div += {"style", "color:green;"};
		div += {"style", "font-size:2em;"};
		div += "Hello! I am a dynamic one."_v;

		std::ostringstream ss;
		auto mod = module_(ss); //begin javascript
		
		//declare a variable of the 'tag' type, and give the variable a specific name for demo
		auto dynamic_tag = mod.make_var<tag_t>("dynamic_tag");
		auto body_tag = mod.make_var<tag_t>();
		mod
			[body_tag = "sample_body"_v] //get the page body by id, "sample_body"
			.if_ (body_tag)
				[dynamic_tag = div] //this will generate JS code to create the 'dyn' tag, and assign the result to dynamic_tag
				[body_tag.append(dynamic_tag)] //append dynamic tag to the page body tag
			.endif_()
			;
		sample.body() += script(ss.str()); //add the script to the page
	}


	//create action buttons
	{
		std::ostringstream js;

		func<void(std::string)> onclick;
		{
			auto body = def_(js, onclick);
			body
				[jstd::alert(onclick.arg<0>())]
				;
		}
		sample.body() += script(js.str());

		//static button
		{
			tag btn{"button"};
			btn += "Click Me!"_v;
			btn += {"onclick", onclick.call_string("I am a static button"_v)};

			sample.body() += btn;
		}

		//dynamic button
		{
			tag btn{"button"};
			btn += "Click Me!"_v;
			btn += {"onclick", onclick.call_string("I am a dynamic button"_v)};

			std::ostringstream ss;
			auto mod = module_(ss); //begin javascript
			
			//declare a variable of the 'tag' type, and give the variable a specific name for demo
			auto dynamic_tag = mod.make_var<tag_t>();
			auto body_tag = mod.make_var<tag_t>();
			mod
				[body_tag = "sample_body"_v] //get the page body by id, "sample_body"
				.if_ (body_tag)
					[dynamic_tag = btn] //this will generate JS code to create the 'dyn' tag, and assign the result to dynamic_tag
					[body_tag.append(dynamic_tag)] //append dynamic tag to the page body tag
				.endif_()
				;
			sample.body() += script(ss.str()); //add the script to the page
		}
	}

	// output the sample page
	std::cout << sample;
	return 0;
}
