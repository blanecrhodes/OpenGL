#pragma once

enum MovementDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

f32 DEF_YAW = -90.0f; 
f32 DEF_PITCH = 0.0f;
f32 DEF_SPEED = 2.5f;
f32 DEF_SENSITIVITY = 0.1f;
f32 DEF_FOV = 45.0f;
glm::vec3 DEF_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 DEF_FACING =  glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 DEF_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

struct Camera {
    glm::vec3 position;
	glm::vec3 facingDirection;
	glm::vec3 upDirection;
    glm::vec3 rightDirection;
    glm::vec3 worldUpDirection;
	f32 pitch;
    f32 yaw;
	f32 fov;
    f32 speed;
    f32 sensitivity;
};

void UpdateCameraVectors(Camera* camera) {
    glm::vec3 newFacing;
    newFacing.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    newFacing.y = sin(glm::radians(camera->pitch));
    newFacing.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->facingDirection = glm::normalize(newFacing);
    camera->rightDirection = glm::normalize(glm::cross(camera->facingDirection, camera->worldUpDirection));
    camera->upDirection = glm::normalize(glm::cross(camera->rightDirection, camera->facingDirection));
}


Camera MakeCamera(glm::vec3 pos, glm::vec3 worldUp) {
    Camera result;
    result.position = pos;
    result.worldUpDirection = worldUp;
    result.yaw = DEF_YAW;
    result.pitch = DEF_PITCH;
    result.facingDirection = DEF_FACING;
    result.speed = DEF_SPEED;
    result.sensitivity = DEF_SENSITIVITY;
    result.fov = DEF_FOV;
    UpdateCameraVectors(&result);
    return result;
}

void UpdatePosition(Camera* camera, glm::vec3 newPos) {
	camera->position = newPos;
}


void UpdatePitch(Camera* camera, f32 pitchOffset) {
    f32 newPitch = camera->pitch + pitchOffset;
    if (newPitch > 89.0f) {
        newPitch = 89.0f;
    }
    if (newPitch < -89.0f) {
        newPitch = -89.0f;
    }
	camera->pitch = newPitch;
}

void UpdateYaw(Camera* camera, f32 yawOffset) {
    camera->yaw += yawOffset;
}

void UpdateFov(Camera* camera, f32 fovOffset) {
    f32 newFov = camera->fov - fovOffset;
    if (newFov < 1.0f) {
        newFov = 1.0f;
    }
    if (newFov > 45.0f) {
        newFov = 45.0f;
    }
	camera->fov = newFov;
}

void UpdateFacingDirection(Camera* camera, f32 pitch, f32 yaw) {
	glm::vec3 newFacing = glm::vec3(1.0f);
    newFacing.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFacing.y = sin(glm::radians(pitch));
    newFacing.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->facingDirection = glm::normalize(newFacing);
}

void UpdateFacingDirection(Camera* camera) {
	glm::vec3 newFacing = glm::vec3(1.0f);
    newFacing.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    newFacing.y = sin(glm::radians(camera->pitch));
    newFacing.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->facingDirection = glm::normalize(newFacing);
}

glm::mat4 GetCameraViewMatrix(Camera* camera) {
    return glm::lookAt(camera->position, camera->position + camera->facingDirection, camera->upDirection);
}

void CameraProcessKeyboard(Camera* camera, MovementDirection direction, f32 deltaTime) {
    float velocity = camera->speed * deltaTime;
    if (direction == FORWARD)
        camera->position += camera->facingDirection * velocity;
    if (direction == BACKWARD)
        camera->position -= camera->facingDirection * velocity;
    if (direction == LEFT)
        camera->position -= camera->rightDirection * velocity;
    if (direction == RIGHT)
        camera->position += camera->rightDirection * velocity;
}

void CameraProcessMouseScroll(Camera* camera, float yoffset) {
    UpdateFov(camera, yoffset);
}

void CameraProcessMouseMovement(Camera* camera, f32 xoffset, f32 yoffset, bool constrainPitch = true) {
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (constrainPitch)
    {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    UpdateCameraVectors(camera);
}
