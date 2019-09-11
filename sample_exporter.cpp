// dont care to make a project with proper include dirs
#include "GlTFExporter.cpp"


int main(int argc, char* argv[])
{
    // input data from
    // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
    std::vector<GlTFExporter::CVector3> vertices =
    {
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
        GlTFExporter::CVector3(-1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
        GlTFExporter::CVector3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
        GlTFExporter::CVector3(1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f,-1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(-1.0f, 1.0f, 1.0f),
        GlTFExporter::CVector3(1.0f,-1.0f, 1.0f)
    };

    // input data from
    // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
    std::vector<GlTFExporter::CVector4> colors =
    {
        GlTFExporter::CVector4(0.583f,  0.771f,  0.014f, 1.0f),
        GlTFExporter::CVector4(0.609f,  0.115f,  0.436f, 1.0f),
        GlTFExporter::CVector4(0.327f,  0.483f,  0.844f, 1.0f),
        GlTFExporter::CVector4(0.822f,  0.569f,  0.201f, 1.0f),
        GlTFExporter::CVector4(0.435f,  0.602f,  0.223f, 1.0f),
        GlTFExporter::CVector4(0.310f,  0.747f,  0.185f, 1.0f),
        GlTFExporter::CVector4(0.597f,  0.770f,  0.761f, 1.0f),
        GlTFExporter::CVector4(0.559f,  0.436f,  0.730f, 1.0f),
        GlTFExporter::CVector4(0.359f,  0.583f,  0.152f, 1.0f),
        GlTFExporter::CVector4(0.483f,  0.596f,  0.789f, 1.0f),
        GlTFExporter::CVector4(0.559f,  0.861f,  0.639f, 1.0f),
        GlTFExporter::CVector4(0.195f,  0.548f,  0.859f, 1.0f),
        GlTFExporter::CVector4(0.014f,  0.184f,  0.576f, 1.0f),
        GlTFExporter::CVector4(0.771f,  0.328f,  0.970f, 1.0f),
        GlTFExporter::CVector4(0.406f,  0.615f,  0.116f, 1.0f),
        GlTFExporter::CVector4(0.676f,  0.977f,  0.133f, 1.0f),
        GlTFExporter::CVector4(0.971f,  0.572f,  0.833f, 1.0f),
        GlTFExporter::CVector4(0.140f,  0.616f,  0.489f, 1.0f),
        GlTFExporter::CVector4(0.997f,  0.513f,  0.064f, 1.0f),
        GlTFExporter::CVector4(0.945f,  0.719f,  0.592f, 1.0f),
        GlTFExporter::CVector4(0.543f,  0.021f,  0.978f, 1.0f),
        GlTFExporter::CVector4(0.279f,  0.317f,  0.505f, 1.0f),
        GlTFExporter::CVector4(0.167f,  0.620f,  0.077f, 1.0f),
        GlTFExporter::CVector4(0.347f,  0.857f,  0.137f, 1.0f),
        GlTFExporter::CVector4(0.055f,  0.953f,  0.042f, 1.0f),
        GlTFExporter::CVector4(0.714f,  0.505f,  0.345f, 1.0f),
        GlTFExporter::CVector4(0.783f,  0.290f,  0.734f, 1.0f),
        GlTFExporter::CVector4(0.722f,  0.645f,  0.174f, 1.0f),
        GlTFExporter::CVector4(0.302f,  0.455f,  0.848f, 1.0f),
        GlTFExporter::CVector4(0.225f,  0.587f,  0.040f, 1.0f),
        GlTFExporter::CVector4(0.517f,  0.713f,  0.338f, 1.0f),
        GlTFExporter::CVector4(0.053f,  0.959f,  0.120f, 1.0f),
        GlTFExporter::CVector4(0.393f,  0.621f,  0.362f, 1.0f),
        GlTFExporter::CVector4(0.673f,  0.211f,  0.457f, 1.0f),
        GlTFExporter::CVector4(0.820f,  0.883f,  0.371f, 1.0f),
        GlTFExporter::CVector4(0.982f,  0.099f,  0.879f, 1.0f)
    };

    std::vector<uint32_t> indices;
    for (auto i = 0; i + 2 < vertices.size(); ++i)
    {
        for (auto j = 0; j < 2; ++j)
        {
            indices.push_back(i);
            indices.push_back(i + j);
            indices.push_back(i + j + 1);
        }
    }

    GlTFExporter exporter;
    
    // build mesh data
    GlTFExporter::MeshPtr pMesh = std::make_shared<GlTFExporter::Mesh>();
    pMesh->name = "cubeMesh";
    pMesh->vertices = vertices; // for larger data consider std::move
    pMesh->indices = indices;   // ^
    pMesh->colors = colors;     // ^
                                // todo: probably take pointer and length instead of vector?
                              

    // model containing the meshes
    GlTFExporter::ModelPtr pModel = std::make_shared<GlTFExporter::Model>();
    pModel->name = "cubeModel";

    // add mesh to model
    exporter.addMeshToModel(pMesh, pModel);

    // groups are currently shit, need a root group added to exporter
    // and subgroups added to the root group via pRootGroup->addSubGroup(pSubGroup)
    GlTFExporter::GroupPtr pRootGroup = std::make_shared<GlTFExporter::Group>();
    pRootGroup->name = "cubeGroup";
    exporter.addGroup(pRootGroup);

    // add this model to the exporter so other nodes can reference it
    exporter.addModel(pModel);

    // means nothing, just to spread shit out
    int shittyCounter = -15;
    for (auto i = 0; i < 3; ++i)
    {
        // check if the group exists, if it does add the models to parent group
        // if not make a new one
        std::string groupName("cubeGroup" + std::to_string(i));
        GlTFExporter::GroupPtr pSubGroup = exporter.getGroup(groupName);
        if (pSubGroup == nullptr)
        {
            pSubGroup = std::make_shared<GlTFExporter::Group>();
            pSubGroup->name = groupName;
            pSubGroup->transformation.translation = GlTFExporter::CVector3(i * 100, i, -i * 100);
        }
        for (auto j = 0; j < 2; ++j)
        {
            for (auto k = 0; k < 2; ++k)
            {
                // add each transformation of the model pointed to by pTransformedModel->modelRef
                // this would be looked up via exporter.getModel(const std::string& name)
                GlTFExporter::TransformedModelPtr pTransformedModel = std::make_shared<GlTFExporter::TransformedModel>();
            
                // add a reference to the model we want to create copies off
                pTransformedModel->modelRef = pModel->id;
            
                // make things move
                pTransformedModel->transformation.translation = GlTFExporter::CVector3((j - 3) * 10 * k, i, (j + 3) * 10 * k);
                pTransformedModel->transformation.scale = GlTFExporter::CVector3(5, 5, 5);
            
                // add this instance to the exporter
                exporter.addTransformedModelToGroup(pTransformedModel, pSubGroup);
                shittyCounter++;
            }
        }
        
        // add subgroup to the root group cause its broken and rootgroup MUST contain all other groups and idk why
        pRootGroup->addGroup(pSubGroup);
    }

    std::string filename("test.gltf");
    std::string outdir("./bin/");
    exporter.doExport(filename, outdir);
    return 0;
}