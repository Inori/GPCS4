Shader

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inPosB;
layout (location = 3) in vec3 inColorB;

...

Application

// Input state
std::array<VkVertexInputBindingDescription,2> vertexInputBinding = {};
vertexInputBinding[0].binding = 0;
vertexInputBinding[0].stride = sizeof(Vertex);
vertexInputBinding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
vertexInputBinding[1].binding = 1;
vertexInputBinding[1].stride = sizeof(Vertex);
vertexInputBinding[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

// Inpute attribute bindings describe shader attribute locations and memory layouts
std::array<VkVertexInputAttributeDescription, 4> vertexInputAttributs;
vertexInputAttributs[0].binding = 0;
vertexInputAttributs[0].location = 0;
vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
vertexInputAttributs[0].offset = offsetof(Vertex, position);
vertexInputAttributs[1].binding = 0;
vertexInputAttributs[1].location = 1;
vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
vertexInputAttributs[1].offset = offsetof(Vertex, color);

vertexInputAttributs[2].binding = 1;
vertexInputAttributs[2].location = 2;
vertexInputAttributs[2].format = VK_FORMAT_R32G32B32_SFLOAT;
vertexInputAttributs[2].offset = offsetof(Vertex, position);
vertexInputAttributs[3].binding = 1;
vertexInputAttributs[3].location = 3;
vertexInputAttributs[3].format = VK_FORMAT_R32G32B32_SFLOAT;
vertexInputAttributs[3].offset = offsetof(Vertex, color);

// Vertex input state used for pipeline creation
VkPipelineVertexInputStateCreateInfo vertexInputState = {};
vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
vertexInputState.vertexBindingDescriptionCount = 2;
vertexInputState.pVertexBindingDescriptions = vertexInputBinding.data();
vertexInputState.vertexAttributeDescriptionCount = 4;
vertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();

// Bind
vkCmdBindVertexBuffers(drawCmdBuffers[i], 0, 1, &vertices.buffer, offsets);
vkCmdBindVertexBuffers(drawCmdBuffers[i], 1, 1, &verticesB.buffer, offsets);
