#include "camera.h"

namespace playground
{
	Camera::Camera(float fovy, float aspect, float zn, float zf)
		: fovy_(fovy), aspect_(aspect), zn_(zn), zf_(zf), dirty_(true)
	{
	}
	
	const Mat4x4f Camera::GetViewMat() const
	{
		if (dirty_)
		{
			dirty_ = false;
			UpdateMat();
		}
		return view_;
	}

	const Mat4x4f Camera::GetViewProjMat() const
	{
		if (dirty_)
		{
			dirty_ = false;
			UpdateMat();
		}
		return view_proj_;
	}

	void Camera::UpdateMat() const
	{
		Mat4x4f proj;
		proj = Mat4x4f::Perspective(fovy_, aspect_, zn_, zf_);
		view_ = (Mat4x4f::Translation(position_) * Mat4x4f::RotationY(yaw_) * Mat4x4f::RotationX(pitch_)).Invert();
		view_proj_ = proj * view_;
	}
}