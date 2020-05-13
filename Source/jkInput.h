#pragma once
#include<string>

// A input will created in BindInput of character class.

class jkInput
{
public:
	jkInput(const std::string& name): mInputName(name), mValue(0.f) {}

	bool operator==(const jkInput& input) const
	{
		return input.mInputName == mInputName;
	}

	std::string GetName() { return mInputName; }
	float GetValue() { return mValue; }

	// Hash functor.
	struct jkInputHash {
		size_t operator()(const jkInput& input) const noexcept {
			return std::hash<std::string>()(input.mInputName);
		}
	};

private:
	std::string mInputName;
	float mValue;
};
