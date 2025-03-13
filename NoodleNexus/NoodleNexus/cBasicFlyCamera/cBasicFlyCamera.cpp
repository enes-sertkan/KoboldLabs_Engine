#include "cBasicFlyCamera.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective



cBasicFlyCamera::cBasicFlyCamera()
{
//	this->m_eye = glm::vec3(0.0f, 0.0f, 0.0f);

	this->m_eye = cBasicFlyCamera::m_FRONT_OF_CAMERA;

	this->m_target = glm::vec3(0.0f, 0.0f, 0.0f);

	this->m_Yaw_Y_axis_rotation = 0.0f;
	this->m_Pitch_X_axis_rotation = 0.0f;

	const float DEFAULT_CAMERA_MOVE_SPEED = 1.0f;
	// At 60Hz (FPS) we'll move 1 degree per second
//	const float DEFAULT_CAMERA_TURN_ANGLE_SPEED_SCALING = 1.0f/60.0f;
	const float DEFAULT_CAMERA_TURN_ANGLE_SPEED_SCALING = 1.0f/600.0f;

	this->m_movementSpeed = DEFAULT_CAMERA_MOVE_SPEED;
	this->m_turnSpeedScaling = DEFAULT_CAMERA_TURN_ANGLE_SPEED_SCALING;

}

// New method to get the view matrix
glm::mat4 cBasicFlyCamera::GetViewMatrix() {
	return glm::lookAt(m_eye, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
}

// New method to get the projection matrix
glm::mat4 cBasicFlyCamera::GetProjectionMatrix(float aspectRatio, float fov) {
	return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

void cBasicFlyCamera::setEyeLocation(glm::vec3 newEyeLocation)
{
	this->m_eye = newEyeLocation;

	// When we update the eye (location), we will also need to update the "target"
	// But we are going to do this when we call getTargetLocation()
	return;
}

void cBasicFlyCamera::setEyeTarget(glm::vec3 newEyeTarget)
{
	this->m_target= newEyeTarget;

	// When we update the eye (location), we will also need to update the "target"
	// But we are going to do this when we call getTargetLocation()
	return;
}


void cBasicFlyCamera::setEyeLocation(float newX, float newY, float newZ)
{
	this->setEyeLocation(glm::vec3(newX, newY, newZ));

	return;
}

void cBasicFlyCamera::setEyeRotation(float x, float y)
{
	this->m_Pitch_X_axis_rotation = glm::radians(x); // Convert pitch from degrees to radians
	this->m_Yaw_Y_axis_rotation = glm::radians(y);   // Convert yaw from degrees to radians


	return;
}



glm::vec3 cBasicFlyCamera::getEyeLocation(void)
{
	return this->m_eye;
}

glm::vec2 cBasicFlyCamera::getEyeRotation(void)
{
	return glm::vec2(glm::degrees(this->m_Pitch_X_axis_rotation), glm::degrees(this->m_Yaw_Y_axis_rotation));
}




glm::vec3 cBasicFlyCamera::getTargetRelativeToCamera(void)
{
	return glm::normalize(this->m_target);
}

glm::vec3 directionToEuler(const glm::vec3& direction) {
	glm::vec3 eulerRotation;

	// Calculate pitch (rotation around X-axis)
	eulerRotation.x = glm::degrees(asin(direction.y));

	// Calculate yaw (rotation around Y-axis)
	eulerRotation.y = glm::degrees(atan2(direction.x, -direction.z));

	// Roll is usually zero unless roll is needed
	eulerRotation.z = 0.0f;

	return eulerRotation;
}

Camera* cBasicFlyCamera::getCameraData()
{
	glm::vec2 resolution = glm::vec2(1920, 1080);

	// Get Camera Position and Target Position
	glm::vec3 eyePos = getEyeLocation();
	glm::vec3 targetPos = getTargetLocation();

	// Compute direction vector
	glm::vec3 direction = glm::normalize(targetPos - eyePos);

	// Convert direction vector to Euler angles (yaw, pitch)
	glm::vec3 rotation = directionToEuler(direction);

	// Create Camera with position & calculated rotation
	Camera* camera = new Camera(eyePos, rotation, resolution, true);
	camera->nightMode = nightMode;
	camera->scene = scene;
	return camera;
}

glm::vec3 cBasicFlyCamera::getTargetLocation(void)
{
	// For now, this is simple, but will get more complicated

//	this->m_target = this->m_eye + cBasicFlyCamera::m_FRONT_OF_CAMERA;
	
	// Calcualte where the target will be, given these angles of rotation:
	// float m_Yaw_Y_axis_rotation;
	// float m_Pitch_X_axis_rotation;

// These lines are taken from the DrawObject, whic is using a 4x4 matrix (mat4 or mat4x4)
//	glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
//									   pCurrentMesh->orientation.x,
//									   glm::vec3(1.0f, 0.0f, 0.0f));
//
//	glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
//									   pCurrentMesh->orientation.y,
//									   glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mat_cameraYaw_Y_Axis = glm::rotate(glm::mat4(1.0f),
												 this->m_Yaw_Y_axis_rotation,
												 glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mat_cameraPitch_X_Axis = glm::rotate(glm::mat4(1.0f),
												   this->m_Pitch_X_axis_rotation,
												   glm::vec3(1.0f, 0.0f, 0.0f));


//	glm::mat3 matFinalOrienation = glm::mat3(1.0f);
	glm::mat3 matFinalOrienation = glm::mat3(mat_cameraYaw_Y_Axis) * glm::mat3(mat_cameraPitch_X_Axis);


//	From the vertex shader
// 	uniform mat4 mModel;
// 	...
//	vertexWorldPosition = mModel * vec4(vPosition.xyz, 1.0f);

	this->m_target = matFinalOrienation * cBasicFlyCamera::m_FRONT_OF_CAMERA;

	glm::vec3 targetInWorldXYZ = this->m_eye + this->m_target;

	return targetInWorldXYZ;
}

void cBasicFlyCamera::rotateLeftRight_Yaw(float yAngleAdjust)
{
	if (freezeRotation) return;
	this->m_Yaw_Y_axis_rotation += (this->m_turnSpeedScaling * yAngleAdjust);
}


void cBasicFlyCamera::rotateLeftRight_Yaw_NoScaling(float yAngleAdjust)
{
	this->m_Yaw_Y_axis_rotation += yAngleAdjust;
}

// positive (+ve) is "looking up"
// negative (-ve) is "looking down"
void cBasicFlyCamera::pitchUpDown(float xAngleAdjust)
{
	if (freezeRotation) return;


	this->m_Pitch_X_axis_rotation += (this->m_turnSpeedScaling * xAngleAdjust);

	// Clamp the pitch to +89 or -89 degrees
	// When we use the lookAt() transform, if the target == up, then
	//	we get a NaN (divide by zero)
	// AND if we keep going, then the pitch reverses because of the up vector
	if ( this->m_Pitch_X_axis_rotation <= glm::radians(-89.0f) )
	{
		this->m_Pitch_X_axis_rotation = glm::radians(-89.0f);
	}
	if ( this->m_Pitch_X_axis_rotation >= glm::radians( 89.0f ) )
	{
		this->m_Pitch_X_axis_rotation = glm::radians(89.0f);
	}

	return;
}


void cBasicFlyCamera::FreezePlayerRotation()
{
	freezeRotation = true;
}

void cBasicFlyCamera::UnfreezePlayerRotation()
{
	freezeRotation = false;
}

// The mouse will pass in integer values, passing "1" per "tick" of the mouse wheel.
// This will likely need to be adjusted per mouse, but who knows?
void cBasicFlyCamera::adjustMovementSpeed(float adjustment)
{
	this->m_movementSpeed += adjustment;

	// See if it's too high or low
	if ( this->m_movementSpeed < cBasicFlyCamera::MIN_MOVEMENT_SPEED )
	{
		this->m_movementSpeed = cBasicFlyCamera::MIN_MOVEMENT_SPEED;
	}
	if (this->m_movementSpeed > cBasicFlyCamera::MAX_MOVEMENT_SPEED)
	{
		this->m_movementSpeed = cBasicFlyCamera::MAX_MOVEMENT_SPEED;
	}
	return;
}

float cBasicFlyCamera::getMovementSpeed(void)
{
	return this->m_movementSpeed;
}


void cBasicFlyCamera::moveForward(float distanceToMove)
{
	float scaledDistance = distanceToMove * this->m_movementSpeed;

//	this->m_eye += glm::vec3(0.0f, 0.0f, scaledDistance);

	// Now we move it in the direction of the target
	// i.e. we are moving "toward" the target
	// Calculate the forward vector from pitch and yaw

	glm::vec3 rotation = glm::radians(getCameraData()->rotation);
	float pitch = rotation.x; // Rotation around the X-axis
	float yaw = rotation.y;   // Rotation around the Y-axis
	float roll = rotation.z;  // Rotation around the Z-axis (roll)



	glm::vec3 forward;
	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);
	forward = glm::normalize(forward);
	this->m_eye += (forward * distanceToMove);

	return;
}


void cBasicFlyCamera::moveLeftRight(float distanceToMove)
{
	float scaledDistance = distanceToMove * this->m_movementSpeed*0.1f;


	glm::vec3 rotation = glm::radians(getCameraData()->rotation);
	float pitch = rotation.x; // Rotation around the X-axis
	float yaw = rotation.y;   // Rotation around the Y-axis
	float roll = rotation.z;  // Rotation around the Z-axis (roll)



	glm::vec3 forward;
	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);
	forward = glm::normalize(forward);

	// Calculate the right vector
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));


	this->m_eye += (-right * scaledDistance);


	return;
}

void cBasicFlyCamera::moveUpDown(float distanceToMove)
{
	float scaledDistance = distanceToMove * this->m_movementSpeed;

	// HACK:
	this->m_eye += glm::vec3(0.0f, scaledDistance, 0.0f);
	return;
}
