#include "Renderer.h"
#include <map>

using namespace std;

#define FORMAT_UNKNOWN 0
#define FORMAT_V 1
#define FORMAT_VT 2
#define FORMAT_VTN 3
#define FORMAT_VN 4

	// struct vaoBuffers will be used inside the rt3d library
	// clients should not need to know about this data structure
	struct vaoBuffers {
		GLuint vertex_buffer;
		GLuint colour_buffer;
		GLuint normal_buffer;
		GLuint texcoord_buffer;
		GLuint index_buffer;
	};

	static map<GLuint, GLuint *> vertexArrayMap;

	SDL_Window* Renderer::createWindow(SDL_GLContext &context)
	{
		SDL_Window* window;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		{
			std::cout << "unable to set up window" << std::endl;
		}
		// Request an OpenGL 3.0 context.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

														   // Create 800x600 window
		window = SDL_CreateWindow("dis is da gam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (!window) // Check window was created OK
		{
			std::cout << "unable to set up window" << std::endl;
		}

		context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
		SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
		return window;
	}

	// Something went wrong - print error message and quit
	void  Renderer::exitFatalError(const char *message) {
		cout << message << " ";
		exit(1);
	}

	// loadFile - loads text file from file fname as a char* 
	// Allocates memory - so remember to delete after use
	// size of file returned in fSize
	char* Renderer::loadFile(const char *fname, GLint &fSize) {
		int size;
		char * memblock;

		// file read based on example in cplusplus.com tutorial
		// ios::ate opens file at the end
		ifstream file(fname, ios::in | ios::binary | ios::ate);
		if (file.is_open()) {
			size = (int)file.tellg(); // get location of file pointer i.e. file size
			fSize = (GLint)size;
			memblock = new char[size];
			file.seekg(0, ios::beg);
			file.read(memblock, size);
			file.close();
			cout << "file " << fname << " loaded" << endl;
		}
		else {
			cout << "Unable to open file " << fname << endl;
			fSize = 0;
			// should ideally set a flag or use exception handling
			// so that calling function can decide what to do now
			return nullptr;
		}
		return memblock;
	}

	// printShaderError
	// Display (hopefully) useful error messages if shader fails to compile or link
	void  Renderer::printShaderError(const GLint shader) {
		int maxLength = 0;
		int logLength = 0;
		GLchar *logMessage;

		// Find out how long the error message is
		if (!glIsShader(shader))
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		else
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength > 0) { // If message has some contents
			logMessage = new GLchar[maxLength];
			if (!glIsShader(shader))
				glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
			else
				glGetShaderInfoLog(shader, maxLength, &logLength, logMessage);
			cout << "Shader Info Log:" << endl << logMessage << endl;
			delete[] logMessage;
		}
		// should additionally check for OpenGL errors here
	}


	GLuint  Renderer::initShaders(const char *vertFile, const char *fragFile) {
		GLuint p, f, v;

		char *vs, *fs;

		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);

		// load shaders & get length of each
		GLint vlen;
		GLint flen;
		vs = loadFile(vertFile, vlen);
		fs = loadFile(fragFile, flen);

		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv, &vlen);
		glShaderSource(f, 1, &ff, &flen);

		GLint compiled;

		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			cout << "Vertex shader not compiled." << endl;
			Renderer::printShaderError(v);
		}

		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			cout << "Fragment shader not compiled." << endl;
			Renderer::printShaderError(f);
		}

		p = glCreateProgram();

		glAttachShader(p, v);
		glAttachShader(p, f);

		glBindAttribLocation(p, RT3D_VERTEX, "in_Position");
		glBindAttribLocation(p, RT3D_COLOUR, "in_Color");
		glBindAttribLocation(p, RT3D_NORMAL, "in_Normal");
		glBindAttribLocation(p, RT3D_TEXCOORD, "in_TexCoord");

		glLinkProgram(p);
		glUseProgram(p);

		delete[] vs; // dont forget to free allocated memory
		delete[] fs; // we allocated this in the loadFile function...

		return p;
	}

	GLuint  Renderer::createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours,
		const GLfloat* normals, const GLfloat* texcoords, const GLuint indexCount, const GLuint* indices) {
		GLuint VAO;
		// generate and set up a VAO for the mesh
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint *pMeshBuffers = new GLuint[5];


		if (vertices == nullptr) {
			// cant create a mesh without vertices... oops
			exitFatalError("Attempt to create a mesh with no vertices");
		}

		// generate and set up the VBOs for the data
		GLuint VBO;
		glGenBuffers(1, &VBO);

		// VBO for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * numVerts * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)RT3D_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(RT3D_VERTEX);
		pMeshBuffers[RT3D_VERTEX] = VBO;


		// VBO for colour data
		if (colours != nullptr) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 3 * numVerts * sizeof(GLfloat), colours, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)RT3D_COLOUR, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(RT3D_COLOUR);
			pMeshBuffers[RT3D_COLOUR] = VBO;
		}

		// VBO for normal data
		if (normals != nullptr) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 3 * numVerts * sizeof(GLfloat), normals, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)RT3D_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(RT3D_NORMAL);
			pMeshBuffers[RT3D_NORMAL] = VBO;
		}

		// VBO for tex-coord data
		if (texcoords != nullptr) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 2 * numVerts * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)RT3D_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(RT3D_TEXCOORD);
			pMeshBuffers[RT3D_TEXCOORD] = VBO;
		}

		if (indices != nullptr && indexCount > 0) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
			pMeshBuffers[RT3D_INDEX] = VBO;
		}
		// unbind vertex array
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// return the identifier needed to draw this mesh

		vertexArrayMap.insert(pair<GLuint, GLuint *>(VAO, pMeshBuffers));

		return VAO;
	}

	GLuint  Renderer::createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours,
		const GLfloat* normals, const GLfloat* texcoords) {
		return createMesh(numVerts, vertices, colours, normals, texcoords, 0, nullptr);
	}

	GLuint  Renderer::createMesh(const GLuint numVerts, const GLfloat* vertices) {
		return createMesh(numVerts, vertices, nullptr, nullptr, nullptr);
	}

	GLuint  Renderer::createColourMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours) {
		return createMesh(numVerts, vertices, colours, nullptr, nullptr);
	}

	void  Renderer::setUniformMatrix4fv(const GLuint program, const char* uniformName, const GLfloat *data) {
		int uniformIndex = glGetUniformLocation(program, uniformName);
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}


	void  Renderer::setLightPos(const GLuint program, const GLfloat *lightPos) {
		int uniformIndex = glGetUniformLocation(program, "lightPosition");
		glUniform4fv(uniformIndex, 1, lightPos);
	}

	void  setProjection(const GLuint program, const GLfloat *data) {
		int uniformIndex = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
	}

	void  Renderer::setLight(const GLuint program, const Renderer::lightStruct light) {
		// pass in light data to shader
		int uniformIndex = glGetUniformLocation(program, "light.ambient");
		glUniform4fv(uniformIndex, 1, light.ambient);
		uniformIndex = glGetUniformLocation(program, "light.diffuse");
		glUniform4fv(uniformIndex, 1, light.diffuse);
		uniformIndex = glGetUniformLocation(program, "light.specular");
		glUniform4fv(uniformIndex, 1, light.specular);
		uniformIndex = glGetUniformLocation(program, "lightPosition");
		glUniform4fv(uniformIndex, 1, light.position);
	}


	void  Renderer::setMaterial(const GLuint program, const Renderer::materialStruct material) {
		// pass in material data to shader 
		int uniformIndex = glGetUniformLocation(program, "material.ambient");
		glUniform4fv(uniformIndex, 1, material.ambient);
		uniformIndex = glGetUniformLocation(program, "material.diffuse");
		glUniform4fv(uniformIndex, 1, material.diffuse);
		uniformIndex = glGetUniformLocation(program, "material.specular");
		glUniform4fv(uniformIndex, 1, material.specular);
		uniformIndex = glGetUniformLocation(program, "material.shininess");
		glUniform1f(uniformIndex, material.shininess);
	}

	void  Renderer::drawMesh(const GLuint mesh, const GLuint numVerts, const GLuint primitive) {
		glBindVertexArray(mesh);	// Bind mesh VAO
		glDrawArrays(primitive, 0, numVerts);	// draw first vertex array object
		glBindVertexArray(0);
	}


	void  Renderer::drawIndexedMesh(const GLuint mesh, const GLuint indexCount, const GLuint primitive) {
		glBindVertexArray(mesh);	// Bind mesh VAO
		glDrawElements(primitive, indexCount, GL_UNSIGNED_INT, 0);	// draw VAO 
		glBindVertexArray(0);
	}


	void  Renderer::updateMesh(const GLuint mesh, const unsigned int bufferType, const GLfloat *data, const GLuint size) {
		GLuint * pMeshBuffers = vertexArrayMap[mesh];
		glBindVertexArray(mesh);

		// Delete the old buffer data
		glDeleteBuffers(1, &pMeshBuffers[bufferType]);

		// generate and set up the VBOs for the new data
		GLuint VBO;
		glGenBuffers(1, &VBO);
		// VBO for the data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)bufferType, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(bufferType);
		pMeshBuffers[RT3D_VERTEX] = VBO;

		glBindVertexArray(0);

	}

	struct position {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};

	struct faceIndex {
		int v;
		int t;
		int n;
	};

	int determineFaceFormat(std::string fString) {
		unsigned int delim1 = fString.find('/');
		if (delim1 == std::string::npos)
			return FORMAT_V;
		unsigned int delim2 = fString.rfind('/');
		if (delim1 == delim2)
			return FORMAT_VT;
		if (delim2 == (delim1 + 1))
			return FORMAT_VN;
		return FORMAT_VTN;
	}

	faceIndex getFace(std::string fString, int fFormat) {
		// still need to sort out how to handle v/t properly, as well as v//n
		int delim1 = fString.find('/');
		int delim2 = fString.rfind('/');

		std::string vstr = fString.substr(0, delim1);
		std::stringstream buffer;
		if (fFormat == FORMAT_VT) {
			// v/t format
			std::string tstr = fString.substr(delim1 + 1, fString.size());
			buffer << vstr << " " << tstr << " " << 0 << " ";
		}
		else {
			// v/t/n format or v//n format
			std::string nstr = fString.substr(delim2 + 1, fString.size() - delim2);
			if (fFormat == FORMAT_VTN) {
				std::string tstr = fString.substr(delim1 + 1, delim2 - (delim1 + 1));
				buffer << vstr << " " << tstr << " " << nstr << " ";
			}
			else
				buffer << vstr << " " << 0 << " " << nstr << " ";
		}

		faceIndex f;
		buffer >> f.v >> f.t >> f.n;
		f.v--; f.t--, f.n--;
		buffer.clear();
		return f;
	}


	void addVertex(std::string fString1, std::map<std::string, GLuint> &indexMap,
		std::vector<position> &inVerts, std::vector<position> &inCoords, std::vector<position> &inNorms,
		std::vector<GLfloat> &verts, std::vector<GLfloat> &texcoords, std::vector<GLfloat> &norms,
		std::vector<GLuint> &indices, int fFormat, int &index) {

		auto itr = indexMap.find(fString1);
		if (itr == indexMap.end()) {
			faceIndex f = getFace(fString1, fFormat);
			verts.push_back(inVerts[f.v].x);
			verts.push_back(inVerts[f.v].y);
			verts.push_back(inVerts[f.v].z);
			if (fFormat < FORMAT_VN) {
				texcoords.push_back(inCoords[f.t].x);
				texcoords.push_back(inCoords[f.t].y);
			}
			if (fFormat > FORMAT_VT) {
				norms.push_back(inNorms[f.n].x);
				norms.push_back(inNorms[f.n].y);
				norms.push_back(inNorms[f.n].z);
			}
			indexMap.insert(std::pair<std::string, GLuint>(fString1, index));
			indices.push_back(index++);
		}
		else {
			indices.push_back(itr->second);
		}
	}



	void  Renderer::loadObj(const char* filename, std::vector<GLfloat> &verts, std::vector<GLfloat> &norms,
		std::vector<GLfloat> &texcoords, std::vector<GLuint> &indices) {

		GLint fileLength;
		char *fileSource = loadFile(filename, fileLength);

		if (fileLength == 0)
			// should report error here too
			return;

		std::stringstream  fileStream;
		fileStream << fileSource;

		char line[256];
		std::string lineHeader;
		std::string fString1;
		std::string fString2;
		std::string fString3;
		std::stringstream buffer;
		std::vector<position> inVerts;
		std::vector<position> inNorms;
		std::vector<position> inCoords;
		//std::vector<GLint> indexVector;

		GLfloat x, y, z;
		GLint a, b, c;

		int i = 0, iCount = 0;
		position tmp;
		std::map<std::string, GLuint> indexMap;
		int fFormat = FORMAT_UNKNOWN;

		std::cout << "started parsing obj image..." << std::endl;

		while (fileStream.good()) {
			//fileStream.getline(line, 256);
			fileStream >> lineHeader;
			switch (lineHeader[0]) {
			case 'v':
				if (lineHeader.length() > 1)
					switch (lineHeader[1]) {
					case 't':
						fileStream >> tmp.x >> tmp.y;
						inCoords.push_back(tmp);
						break;
					case 'n':
						fileStream >> tmp.x >> tmp.y >> tmp.z;
						inNorms.push_back(tmp);
						break;
					default:
						break;
					}
				else {
					fileStream >> tmp.x >> tmp.y >> tmp.z;
					inVerts.push_back(tmp);
				}
				break;
			case 'f':
				fileStream >> fString1 >> fString2 >> fString3;
				if (!fFormat)
					fFormat = determineFaceFormat(fString1);
				if (fFormat > FORMAT_V) {
					addVertex(fString1, indexMap, inVerts, inCoords, inNorms, verts, texcoords, norms, indices, fFormat, iCount);
					addVertex(fString2, indexMap, inVerts, inCoords, inNorms, verts, texcoords, norms, indices, fFormat, iCount);
					addVertex(fString3, indexMap, inVerts, inCoords, inNorms, verts, texcoords, norms, indices, fFormat, iCount);
				}
				else {
					buffer << fString1 << " " << fString2 << " " << fString3;
					buffer >> a >> b >> c;
					buffer.clear();
					indices.push_back(a - 1);
					indices.push_back(b - 1);
					indices.push_back(c - 1);
				}
				break;
			case '#':
				fileStream.getline(line, 256); // ignore line
				break;
			default:
				fileStream.getline(line, 256); // ignore line
			}

		}

		// copy vertex data to output vectors in case only single index was provided....
		if (fFormat == FORMAT_V) {
			for (int v = 0; v < inVerts.size(); v++) {
				verts.push_back(inVerts[v].x);
				verts.push_back(inVerts[v].y);
				verts.push_back(inVerts[v].z);
			}
		}

		std::cout << "finished parsing obj image..." << std::endl;


	}

	GLuint  Renderer::loadBitmap(char * name)
	{
		GLuint texID;
		glGenTextures(1, &texID); // generate texture ID

								  // load file - using core SDL library
		SDL_Surface *tmpSurface;
		tmpSurface = SDL_LoadBMP(name);
		if (!tmpSurface) {
			std::cout << "Error loading bitmap" << std::endl;
		}

		// bind texture and set parameters
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SDL_PixelFormat *format = tmpSurface->format;

		GLuint externalFormat, internalFormat;
		if (format->Amask) {
			internalFormat = GL_RGBA;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
		}
		else {
			internalFormat = GL_RGB;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
		return texID;	// return value of texture ID
	}
