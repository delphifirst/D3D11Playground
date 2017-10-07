#pragma once

#include "vec.h"
#include "mat.h"

namespace playground
{
	class Camera
	{
	public:
		Camera(float fovy, float aspect, float zn, float zf);

		float GetFovY() const
		{
			return fovy_;
		}

		float GetAspect() const
		{
			return aspect_;
		}

		float GetZN() const
		{
			return zn_;
		}

		float GetZF() const
		{
			return zf_;
		}

		const Vec3f GetPosition() const
		{
			return position_;
		}

		float GetYaw() const
		{
			return yaw_;
		}

		float GetPitch() const
		{
			return pitch_;
		}

		const Mat4x4f GetViewMat() const;
		const Mat4x4f GetViewProjMat() const;

		void SetFovY(float fovy)
		{
			fovy_ = fovy;
			dirty_ = true;
		}

		void SetAspect(float aspect)
		{
			aspect_ = aspect;
			dirty_ = true;
		}

		void SetZN(float zn)
		{
			zn_ = zn;
			dirty_ = true;
		}

		void SetZF(float zf)
		{
			zf_ = zf;
			dirty_ = true;
		}

		void SetPosition(const Vec3f& position)
		{
			position_ = position;
			dirty_ = true;
		}

		void SetYaw(float yaw)
		{
			yaw_ = yaw;
			dirty_ = true;
		}

		void SetPitch(float pitch)
		{
			pitch_ = pitch;
			dirty_ = true;
		}
	private:
		void UpdateMat() const;

		float fovy_, aspect_, zn_, zf_;
		Vec3f position_;
		float yaw_ = 0, pitch_ = 0;

		mutable bool dirty_;
		mutable Mat4x4f view_, view_proj_;
	};
}