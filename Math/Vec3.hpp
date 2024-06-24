#pragma once
struct Vec2;
struct Vec3{
	public: 
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		const static Vec3 ZERO;
		Vec3() {};
		explicit Vec3(float x, float y, float z);
		explicit Vec3(Vec2 copyFrom);
		const Vec3	operator-(const float floatToSubtract) const;	// vec3 - float
		const Vec3	operator+(const float floatToAdd) const;	// vec3 + float

		//void  operator=(const Vec3 floatToAdd);	// vec3 + float
		void	SetFromText(const char* text);
		void	SetFromTextWithSpace(const char* text);
		void	SetFromTextWithDelimiter(const char* text, char delimiter);

		//Accessors
		float GetLength() const;
		float GetLengthXY() const;
		float GetLengthXYSquared() const;
		float GetLengthSquared() const;
		float GetAngleAboutZRadians() const;
		float GetAngleAboutZDegrees() const;
		Vec3 const GetRotatedAboutZRadians(float deltaRadians) const;
		Vec3 const GetRotatedAboutZDegrees(float deltaDegrees) const;
		Vec3 const GetClamped(float maxLength) const;
		Vec3 const GetNormalized() const;

		//operators (Self Mutating Non Const)
		bool		operator==(Vec3 const& compare) const;
		bool		operator!=(Vec3 const& compare) const;
		bool		operator<(Vec3 const& compare) const;
		bool		operator>(Vec3 const& compare) const;
		Vec3 const	operator*(const Vec3& vecToMultiply) const;	
		Vec3 const	operator+(Vec3 const& VecToAdd) const;
		Vec3 const	operator-(Vec3 const& VecToSubtract) const;
		Vec3 const  operator*(float uniformScale) const;
		const Vec3	operator/(float inverseScale) const;		

		//Operators (self-mutating / non-const)
		void		operator+=(const Vec3& vecToAdd);			
		void		operator-=(const Vec3& vecToSubtract);		
		void		operator*=(const float uniformScale);		
		void		operator/=(const float uniformDivisor);		
		void		operator=(const Vec3& copyFrom);		

		void		operator+=(const float& floatToAdd);
		void		operator-=(const float& floatToSubtract);
		const		Vec3	operator-() const;
		//Standalone friend functions
		friend Vec3 const operator*(float uniformScale, Vec3 const& VecToScale);
};