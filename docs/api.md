# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Game`](#class_game) | 
`struct `[`AssetBuffer`](#struct_asset_buffer) | 
`struct `[`AssetLoader`](#struct_asset_loader) | 
`struct `[`AudioModule`](#struct_audio_module) | 
`struct `[`DynamicLibrary`](#struct_dynamic_library) | 
`struct `[`GameContext`](#struct_game_context) | 
`struct `[`GraphicsModule`](#struct_graphics_module) | Graphics module class.
`struct `[`mat4.__unnamed13__`](#structmat4_8____unnamed13____) | 
`struct `[`Module`](#struct_module) | 
`struct `[`sAudioClip`](#structs_audio_clip) | 
`struct `[`sAudioSource`](#structs_audio_source) | 
`struct `[`sCamera`](#structs_camera) | 
`struct `[`Script`](#struct_script) | 
`struct `[`ScriptLoaderModule`](#struct_script_loader_module) | 
`struct `[`sFont`](#structs_font) | 
`struct `[`ShaderModule`](#struct_shader_module) | [ShaderModule](#struct_shader_module) is a module that provides functionality for compiling and creating shaders.
`struct `[`sMesh`](#structs_mesh) | Mesh structure.
`struct `[`sModelTransform`](#structs_model_transform) | 
`struct `[`sShader`](#structs_shader) | Shader structure.
`struct `[`sShaderProgram`](#structs_shader_program) | Shader program structure.
`struct `[`sText`](#structs_text) | 
`struct `[`sTexture`](#structs_texture) | Texture structure.
`struct `[`sTextureDefinition`](#structs_texture_definition) | Texture definition structure.
`struct `[`sUniformDefinition`](#structs_uniform_definition) | Uniform definition structure.
`struct `[`sUniformElement`](#structs_uniform_element) | Uniform element structure.
`struct `[`sUniforms`](#structs_uniforms) | Uniforms structure.
`struct `[`sVertexDefinition`](#structs_vertex_definition) | A single abstract vertex definition.
`struct `[`swAudio`](#structsw_audio) | 
`struct `[`swEtc`](#structsw_etc) | 
`struct `[`swGame`](#structsw_game) | 
`struct `[`sWindow`](#structs_window) | Window structure.
`struct `[`sWindowFlags`](#structs_window_flags) | 
`struct `[`swLevel`](#structsw_level) | 
`struct `[`swLevelObject`](#structsw_level_object) | 
`struct `[`swMaterial`](#structsw_material) | 
`struct `[`swModel`](#structsw_model) | 
`struct `[`swScript`](#structsw_script) | 
`struct `[`swTexture`](#structsw_texture) | 
`struct `[`swWorld`](#structsw_world) | 
`struct `[`TextAssetBuffer`](#struct_text_asset_buffer) | 
`struct `[`TextModule`](#struct_text_module) | 
`struct `[`TextureModule`](#struct_texture_module) | Texture loading module class.
`struct `[`vec2.__unnamed11__`](#structvec2_8____unnamed11____) | 
`struct `[`vec2.__unnamed9__`](#structvec2_8____unnamed9____) | 
`struct `[`vec3.__unnamed1__`](#structvec3_8____unnamed1____) | 
`struct `[`vec3.__unnamed3__`](#structvec3_8____unnamed3____) | 
`struct `[`vec4.__unnamed5__`](#structvec4_8____unnamed5____) | 
`struct `[`vec4.__unnamed7__`](#structvec4_8____unnamed7____) | 
`struct `[`Vertex`](#struct_vertex) | 
`struct `[`WindowModule`](#struct_window_module) | Window module class.
`struct `[`WorldModule`](#struct_world_module) | 
`union `[`mat4`](#unionmat4) | 
`union `[`vec2`](#unionvec2) | 
`union `[`vec3`](#unionvec3) | 
`union `[`vec4`](#unionvec4) | 

# class `Game` <a id="class_game"></a>

```
class Game
  : private Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public GameMain `[`main`](#class_game_1a8ad7e4256e9e9379c5966365504ba751) | 
`public inline  `[`Game`](#class_game_1ad59df6562a58a614fda24622d3715b65)`()` | 

## Members

#### `public GameMain `[`main`](#class_game_1a8ad7e4256e9e9379c5966365504ba751) <a id="class_game_1a8ad7e4256e9e9379c5966365504ba751"></a>

#### `public inline  `[`Game`](#class_game_1ad59df6562a58a614fda24622d3715b65)`()` <a id="class_game_1ad59df6562a58a614fda24622d3715b65"></a>

# struct `AssetBuffer` <a id="struct_asset_buffer"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public const uint8_t * `[`data`](#struct_asset_buffer_1a5628ea7dea03b763da086645d5b57c5a) | 
`public size_t `[`len`](#struct_asset_buffer_1a792420415a5540a2ed57d1f6d8d3eebd) | 

## Members

#### `public const uint8_t * `[`data`](#struct_asset_buffer_1a5628ea7dea03b763da086645d5b57c5a) <a id="struct_asset_buffer_1a5628ea7dea03b763da086645d5b57c5a"></a>

#### `public size_t `[`len`](#struct_asset_buffer_1a792420415a5540a2ed57d1f6d8d3eebd) <a id="struct_asset_buffer_1a792420415a5540a2ed57d1f6d8d3eebd"></a>

# struct `AssetLoader` <a id="struct_asset_loader"></a>

```
struct AssetLoader
  : public Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  `[`AssetLoader`](#struct_asset_loader_1ae090ac88a0e6df2c7cc560da9f4c4f25)`()` | 
`public inline AssetBuffer `[`loadAsset`](#struct_asset_loader_1aefc1c811522ac04f63ca4dd6dcdffc61)`(const char * path)` | 
`public inline TextAssetBuffer `[`loadTextAsset`](#struct_asset_loader_1ac36e9ddc0bfecc543910eecbf24d1164)`(const char * path)` | 

## Members

#### `public inline  `[`AssetLoader`](#struct_asset_loader_1ae090ac88a0e6df2c7cc560da9f4c4f25)`()` <a id="struct_asset_loader_1ae090ac88a0e6df2c7cc560da9f4c4f25"></a>

#### `public inline AssetBuffer `[`loadAsset`](#struct_asset_loader_1aefc1c811522ac04f63ca4dd6dcdffc61)`(const char * path)` <a id="struct_asset_loader_1aefc1c811522ac04f63ca4dd6dcdffc61"></a>

#### `public inline TextAssetBuffer `[`loadTextAsset`](#struct_asset_loader_1ac36e9ddc0bfecc543910eecbf24d1164)`(const char * path)` <a id="struct_asset_loader_1ac36e9ddc0bfecc543910eecbf24d1164"></a>

# struct `AudioModule` <a id="struct_audio_module"></a>

```
struct AudioModule
  : public Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public audio::Init `[`init`](#struct_audio_module_1af2d4c11e300e4b3a09e5237212263964) | 
`public audio::LoadAudioClip `[`loadAudioClip`](#struct_audio_module_1a01d235383a925f6c3a57d59a1d6cf7ef) | 
`public audio::CreateAudioSource `[`createAudioSource`](#struct_audio_module_1a818454355dabbf79b85ef2f96c932fc7) | 
`public audio::PlayAudioSource `[`playAudioSource`](#struct_audio_module_1a97197da94032e3ac8667f2e44a33b235) | 
`public audio::StopAudioSource `[`stopAudioSource`](#struct_audio_module_1aa518453ead8b603b0393ed1e731a61a7) | 
`public audio::SetAudioSourcePosition `[`setAudioSourcePosition`](#struct_audio_module_1a9cd14699e885aa2c6573c7fde948f391) | 
`public audio::SetAudioSourceVelocity `[`setAudioSourceVelocity`](#struct_audio_module_1af239ae2b61b655f2343ac13514a165d0) | 
`public audio::SetAudioSourcePitch `[`setAudioSourcePitch`](#struct_audio_module_1a5cc3713b600f5303be209c4532030d47) | 
`public audio::SetAudioSourceGain `[`setAudioSourceGain`](#struct_audio_module_1a683dbbfffc0089c46f246e59ca393414) | 
`public audio::SetAudioSourceLooping `[`setAudioSourceLooping`](#struct_audio_module_1a81b3cce015e0172673a5def572bc7243) | 
`public audio::SeekAudioSourceSamples `[`seekAudioSourceSamples`](#struct_audio_module_1af25089cd817587419ae41d816360fbea) | 
`public audio::GetAudioSourceSamples `[`getAudioSourceSamples`](#struct_audio_module_1a844e2307439aaef5aef3209ef20708c1) | 
`public audio::GetAudioSourceSampleRate `[`getAudioSourceSampleRate`](#struct_audio_module_1ad0d23f1a570973c859840aa9323eaff8) | 
`public audio::DestroyAudioClip `[`destroyAudioClip`](#struct_audio_module_1a7e1386698c1de4aafcba740758bbd8e6) | 
`public audio::DestroyAudioSource `[`destroyAudioSource`](#struct_audio_module_1a3f619fd63dc47aa31e667c2e841046d5) | 
`public audio::Destroy `[`destroy`](#struct_audio_module_1a4fadeb73308206638870e9d9a29530fc) | 
`public inline void `[`seekAudioSourceSeconds`](#struct_audio_module_1ac597822f242cb394a3b3f28aeceac80b)`(sAudioSource source,float seconds)` | 
`public inline void `[`seekAudioSourcePercent`](#struct_audio_module_1a4a678b99ce96bcfd884de6620424f02d)`(sAudioSource source,float percent)` | 
`public inline float `[`getAudioSourcePercent`](#struct_audio_module_1a99a5fbdf199d922a0117ae56b441c8c0)`(sAudioSource source)` | 
`public inline float `[`getAudioSourceSeconds`](#struct_audio_module_1a192f38e52ecbde6f351043d6651cf2a8)`(sAudioSource source)` | 
`public inline  explicit `[`AudioModule`](#struct_audio_module_1a73e7cc683655fab3fe6534718eff1bb9)`(const char * dylib)` | 

## Members

#### `public audio::Init `[`init`](#struct_audio_module_1af2d4c11e300e4b3a09e5237212263964) <a id="struct_audio_module_1af2d4c11e300e4b3a09e5237212263964"></a>

#### `public audio::LoadAudioClip `[`loadAudioClip`](#struct_audio_module_1a01d235383a925f6c3a57d59a1d6cf7ef) <a id="struct_audio_module_1a01d235383a925f6c3a57d59a1d6cf7ef"></a>

#### `public audio::CreateAudioSource `[`createAudioSource`](#struct_audio_module_1a818454355dabbf79b85ef2f96c932fc7) <a id="struct_audio_module_1a818454355dabbf79b85ef2f96c932fc7"></a>

#### `public audio::PlayAudioSource `[`playAudioSource`](#struct_audio_module_1a97197da94032e3ac8667f2e44a33b235) <a id="struct_audio_module_1a97197da94032e3ac8667f2e44a33b235"></a>

#### `public audio::StopAudioSource `[`stopAudioSource`](#struct_audio_module_1aa518453ead8b603b0393ed1e731a61a7) <a id="struct_audio_module_1aa518453ead8b603b0393ed1e731a61a7"></a>

#### `public audio::SetAudioSourcePosition `[`setAudioSourcePosition`](#struct_audio_module_1a9cd14699e885aa2c6573c7fde948f391) <a id="struct_audio_module_1a9cd14699e885aa2c6573c7fde948f391"></a>

#### `public audio::SetAudioSourceVelocity `[`setAudioSourceVelocity`](#struct_audio_module_1af239ae2b61b655f2343ac13514a165d0) <a id="struct_audio_module_1af239ae2b61b655f2343ac13514a165d0"></a>

#### `public audio::SetAudioSourcePitch `[`setAudioSourcePitch`](#struct_audio_module_1a5cc3713b600f5303be209c4532030d47) <a id="struct_audio_module_1a5cc3713b600f5303be209c4532030d47"></a>

#### `public audio::SetAudioSourceGain `[`setAudioSourceGain`](#struct_audio_module_1a683dbbfffc0089c46f246e59ca393414) <a id="struct_audio_module_1a683dbbfffc0089c46f246e59ca393414"></a>

#### `public audio::SetAudioSourceLooping `[`setAudioSourceLooping`](#struct_audio_module_1a81b3cce015e0172673a5def572bc7243) <a id="struct_audio_module_1a81b3cce015e0172673a5def572bc7243"></a>

#### `public audio::SeekAudioSourceSamples `[`seekAudioSourceSamples`](#struct_audio_module_1af25089cd817587419ae41d816360fbea) <a id="struct_audio_module_1af25089cd817587419ae41d816360fbea"></a>

#### `public audio::GetAudioSourceSamples `[`getAudioSourceSamples`](#struct_audio_module_1a844e2307439aaef5aef3209ef20708c1) <a id="struct_audio_module_1a844e2307439aaef5aef3209ef20708c1"></a>

#### `public audio::GetAudioSourceSampleRate `[`getAudioSourceSampleRate`](#struct_audio_module_1ad0d23f1a570973c859840aa9323eaff8) <a id="struct_audio_module_1ad0d23f1a570973c859840aa9323eaff8"></a>

#### `public audio::DestroyAudioClip `[`destroyAudioClip`](#struct_audio_module_1a7e1386698c1de4aafcba740758bbd8e6) <a id="struct_audio_module_1a7e1386698c1de4aafcba740758bbd8e6"></a>

#### `public audio::DestroyAudioSource `[`destroyAudioSource`](#struct_audio_module_1a3f619fd63dc47aa31e667c2e841046d5) <a id="struct_audio_module_1a3f619fd63dc47aa31e667c2e841046d5"></a>

#### `public audio::Destroy `[`destroy`](#struct_audio_module_1a4fadeb73308206638870e9d9a29530fc) <a id="struct_audio_module_1a4fadeb73308206638870e9d9a29530fc"></a>

#### `public inline void `[`seekAudioSourceSeconds`](#struct_audio_module_1ac597822f242cb394a3b3f28aeceac80b)`(sAudioSource source,float seconds)` <a id="struct_audio_module_1ac597822f242cb394a3b3f28aeceac80b"></a>

#### `public inline void `[`seekAudioSourcePercent`](#struct_audio_module_1a4a678b99ce96bcfd884de6620424f02d)`(sAudioSource source,float percent)` <a id="struct_audio_module_1a4a678b99ce96bcfd884de6620424f02d"></a>

#### `public inline float `[`getAudioSourcePercent`](#struct_audio_module_1a99a5fbdf199d922a0117ae56b441c8c0)`(sAudioSource source)` <a id="struct_audio_module_1a99a5fbdf199d922a0117ae56b441c8c0"></a>

#### `public inline float `[`getAudioSourceSeconds`](#struct_audio_module_1a192f38e52ecbde6f351043d6651cf2a8)`(sAudioSource source)` <a id="struct_audio_module_1a192f38e52ecbde6f351043d6651cf2a8"></a>

#### `public inline  explicit `[`AudioModule`](#struct_audio_module_1a73e7cc683655fab3fe6534718eff1bb9)`(const char * dylib)` <a id="struct_audio_module_1a73e7cc683655fab3fe6534718eff1bb9"></a>

# struct `DynamicLibrary` <a id="struct_dynamic_library"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`handle`](#struct_dynamic_library_1a49bee1e919b86c8b92bf801e433ed8ff) | 
`public const char * `[`mod_name`](#struct_dynamic_library_1a73a46cd7644aa42c16d61c447aa39e44) | 
`public const char * `[`mod_imp`](#struct_dynamic_library_1a6a8fe24283e97641dca9612be64b34ae) | 
`public inline  `[`DynamicLibrary`](#struct_dynamic_library_1a5068283b12c51f6f3589f18ba7075494)`()` | 
`public inline  `[`DynamicLibrary`](#struct_dynamic_library_1a7793f674c0ca06e9edc702aa404fedd3)`(const char * path,const char * ident)` | 
`public inline  `[`~DynamicLibrary`](#struct_dynamic_library_1a25dd7a7cd386fbc1c8abde597c39c881)`()` | 
`public inline void * `[`getSymbol`](#struct_dynamic_library_1a74cdae708163e43a26b410e5d1861ed0)`(const char * name)` | 
`public inline bool `[`valid`](#struct_dynamic_library_1ae708350e2f9a9c34ff6361ebdda4c72d)`()` | 

## Members

#### `public void * `[`handle`](#struct_dynamic_library_1a49bee1e919b86c8b92bf801e433ed8ff) <a id="struct_dynamic_library_1a49bee1e919b86c8b92bf801e433ed8ff"></a>

#### `public const char * `[`mod_name`](#struct_dynamic_library_1a73a46cd7644aa42c16d61c447aa39e44) <a id="struct_dynamic_library_1a73a46cd7644aa42c16d61c447aa39e44"></a>

#### `public const char * `[`mod_imp`](#struct_dynamic_library_1a6a8fe24283e97641dca9612be64b34ae) <a id="struct_dynamic_library_1a6a8fe24283e97641dca9612be64b34ae"></a>

#### `public inline  `[`DynamicLibrary`](#struct_dynamic_library_1a5068283b12c51f6f3589f18ba7075494)`()` <a id="struct_dynamic_library_1a5068283b12c51f6f3589f18ba7075494"></a>

#### `public inline  `[`DynamicLibrary`](#struct_dynamic_library_1a7793f674c0ca06e9edc702aa404fedd3)`(const char * path,const char * ident)` <a id="struct_dynamic_library_1a7793f674c0ca06e9edc702aa404fedd3"></a>

#### `public inline  `[`~DynamicLibrary`](#struct_dynamic_library_1a25dd7a7cd386fbc1c8abde597c39c881)`()` <a id="struct_dynamic_library_1a25dd7a7cd386fbc1c8abde597c39c881"></a>

#### `public inline void * `[`getSymbol`](#struct_dynamic_library_1a74cdae708163e43a26b410e5d1861ed0)`(const char * name)` <a id="struct_dynamic_library_1a74cdae708163e43a26b410e5d1861ed0"></a>

#### `public inline bool `[`valid`](#struct_dynamic_library_1ae708350e2f9a9c34ff6361ebdda4c72d)`()` <a id="struct_dynamic_library_1ae708350e2f9a9c34ff6361ebdda4c72d"></a>

# struct `GameContext` <a id="struct_game_context"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`WindowModule`](#struct_window_module)` `[`winm`](#struct_game_context_1adf7f1d5403d8c252c64e942e61cea40e) | 
`public `[`GraphicsModule`](#struct_graphics_module)` `[`gfxm`](#struct_game_context_1a98856d42044a8ad116a14ff4b0691dde) | 
`public `[`ShaderModule`](#struct_shader_module)` `[`shdr`](#struct_game_context_1a18f6ad3eb96d8c8d81ceff1383ce1f3c) | 
`public `[`TextureModule`](#struct_texture_module)` `[`texm`](#struct_game_context_1a4a5a84d1863f95f95bff9e04762eb132) | 
`public TextModule `[`textm`](#struct_game_context_1a146dcd5835f865af5cf53e2ec30adf87) | 
`public AssetLoader `[`assetm`](#struct_game_context_1a57fe2af4cbf219067eadfb06b6334bf4) | 

## Members

#### `public `[`WindowModule`](#struct_window_module)` `[`winm`](#struct_game_context_1adf7f1d5403d8c252c64e942e61cea40e) <a id="struct_game_context_1adf7f1d5403d8c252c64e942e61cea40e"></a>

#### `public `[`GraphicsModule`](#struct_graphics_module)` `[`gfxm`](#struct_game_context_1a98856d42044a8ad116a14ff4b0691dde) <a id="struct_game_context_1a98856d42044a8ad116a14ff4b0691dde"></a>

#### `public `[`ShaderModule`](#struct_shader_module)` `[`shdr`](#struct_game_context_1a18f6ad3eb96d8c8d81ceff1383ce1f3c) <a id="struct_game_context_1a18f6ad3eb96d8c8d81ceff1383ce1f3c"></a>

#### `public `[`TextureModule`](#struct_texture_module)` `[`texm`](#struct_game_context_1a4a5a84d1863f95f95bff9e04762eb132) <a id="struct_game_context_1a4a5a84d1863f95f95bff9e04762eb132"></a>

#### `public TextModule `[`textm`](#struct_game_context_1a146dcd5835f865af5cf53e2ec30adf87) <a id="struct_game_context_1a146dcd5835f865af5cf53e2ec30adf87"></a>

#### `public AssetLoader `[`assetm`](#struct_game_context_1a57fe2af4cbf219067eadfb06b6334bf4) <a id="struct_game_context_1a57fe2af4cbf219067eadfb06b6334bf4"></a>

# struct `GraphicsModule` <a id="struct_graphics_module"></a>

```
struct GraphicsModule
  : public Module
```  

Graphics module class.

This class represents the graphics module that is used to render graphics to the screen. It contains all the functions that are used to create and use shaders, meshes, and textures. It is used to initialize the graphics module and create graphics objects.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`sWindow`](#structs_window)` * `[`win`](#struct_graphics_module_1a64839384fd14c416875122e01dc37c28) | 
`public inline void `[`init`](#struct_graphics_module_1a72054b7c7196a3d2e978a1656451b7ff)`(`[`sWindow`](#structs_window)` * win)` | Initialize the graphics module.
`public inline void `[`setClearColor`](#struct_graphics_module_1a88a229195b716d94ab78293793d8d674)`(float r,float g,float b,float a)` | Set the clear color for the graphics module.
`public inline void `[`clear`](#struct_graphics_module_1aac7df8fed5ff4e0e08fe3b00d5dc9367)`()` | Clear the screen.
`public inline void `[`present`](#struct_graphics_module_1a75745dcb3eea40c69ccb76436af9dfd2)`()` | Present the rendered frame to the screen.
`public inline void `[`setScissor`](#struct_graphics_module_1a4a1a7bfac82e8d1386e42715fb3faf65)`(int x,int y,int width,int height)` | Set the scissor rectangle for rendering.
`public inline void `[`enableScissor`](#struct_graphics_module_1a3ab028b9d6531ed4625089aeff8a90b3)`()` | Enable the scissor rectangle for rendering.
`public inline void `[`disableScissor`](#struct_graphics_module_1a14ccfafb0a35746d6fa545465b3ce8e8)`()` | Disable the scissor rectangle for rendering.
`public inline void `[`useShaderProgram`](#struct_graphics_module_1ae91c58bf252479f295203992abc89544)`(`[`sShaderProgram`](#structs_shader_program)` program)` | Use a shader program for rendering.
`public inline void `[`drawMesh`](#struct_graphics_module_1a64d05631f87ad6b1bcc2819867531dd7)`(`[`sMesh`](#structs_mesh)` mesh)` | Draw a mesh to the screen.
`public inline void `[`freeMesh`](#struct_graphics_module_1a68fb50ee03efd2df6ec267590ab53484)`(`[`sMesh`](#structs_mesh)` mesh)` | Free a mesh.
`public inline void `[`freeShader`](#struct_graphics_module_1a0fc404fb66e2655dd396eb0f525de5b2)`(`[`sShader`](#structs_shader)` shader)` | Free a shader.
`public inline void `[`freeShaderProgram`](#struct_graphics_module_1af5ca2086152703a32c69414b11e6cc57)`(`[`sShaderProgram`](#structs_shader_program)` program)` | Free a shader program.
`public inline void `[`freeUniforms`](#struct_graphics_module_1a3f4db4305879aaf22e35fa5c58dfd968)`(`[`sUniforms`](#structs_uniforms)` uniforms)` | Free a uniforms object.
`public inline void `[`freeTexture`](#struct_graphics_module_1a809f3292e3f94c6702dbb2020143f38d)`(`[`sTexture`](#structs_texture)` texture)` | Free a texture.
`public inline void `[`destroy`](#struct_graphics_module_1a472b7333e889e2c0510512a1a41835d5)`()` | Destroy the graphics module.
`public inline `[`sTexture`](#structs_texture)` `[`createTexture`](#struct_graphics_module_1a2bdacd3c01063fd53d832f3a29b26477)`(`[`sTextureDefinition`](#structs_texture_definition)` def)` | Create a texture from a texture definition.
`public inline `[`sUniforms`](#structs_uniforms)` `[`createUniforms`](#struct_graphics_module_1a2f08045dbfe589f985952d13f82fd284)`(`[`sShaderProgram`](#structs_shader_program)` program,`[`sUniformDefinition`](#structs_uniform_definition)` def)` | Create a uniforms object from a shader program and a uniform definition.
`public inline void `[`setUniforms`](#struct_graphics_module_1a51fdd18499e304a05d7ae1886b868a4f)`(`[`sUniforms`](#structs_uniforms)` uniforms,void * data)` | Set the uniforms for a shader program.
`public inline void `[`useTexture`](#struct_graphics_module_1a0f53efb3be6f4f0d50a3a5355df62d78)`(`[`sShaderProgram`](#structs_shader_program)` program,`[`sTexture`](#structs_texture)` texture,const char * name)` | Use a texture in a shader program.
`public inline const char * `[`getShaderType`](#struct_graphics_module_1abdb3a71f5aa4c1baa783a4d8cf57a9ed)`()` | Get the shader type as a string.
`public inline `[`sMesh`](#structs_mesh)` `[`createMesh`](#struct_graphics_module_1a2080f1c1f12e7fe2b86045e099820390)`(`[`sShader`](#structs_shader)` vertexShader,void * vertices,size_t vertexCount,`[`sIndex`](#gfx_2module_8h_1aeec0f5b0bda581fc5280ed5a264d3da9)` * indices,size_t indexCount)` | Create a mesh from a vertex shader, vertices, and indices.
`public inline `[`sShaderProgram`](#structs_shader_program)` `[`createShaderProgram`](#struct_graphics_module_1a1291b5f0b13b4883ed273b2f41501bed)`(`[`sShader`](#structs_shader)` * shaders,size_t count)` | Create a shader program from an array of shaders.
`public inline `[`sShaderProgram`](#structs_shader_program)` `[`createShaderProgram`](#struct_graphics_module_1a5364ee0b194d3e4c440c9f3fd79a0cec)`(std::initializer_list< `[`sShader`](#structs_shader)` > shaders)` | Create a shader program from an initializer list of shaders.
`public inline `[`sVertexDefinition`](#structs_vertex_definition)` * `[`createVertexDefinition`](#struct_graphics_module_1a0e870b8170deb3edf109c555d621a059)`(int * elements,size_t count)` | Create a vertex definition from an array of elements.
`public inline `[`sVertexDefinition`](#structs_vertex_definition)` * `[`createVertexDefinition`](#struct_graphics_module_1abcd20d04203631a0165af2d7a7ea6791)`(std::initializer_list< int > elements)` | Create a vertex definition from an initializer list of elements.
`public inline void `[`freeVertexDefinition`](#struct_graphics_module_1a5edbde39320040e354485c88d17fcba8)`(`[`sVertexDefinition`](#structs_vertex_definition)` * def)` | Free a vertex definition.
`public inline `[`sShader`](#structs_shader)` `[`createShader`](#struct_graphics_module_1ab53c3f4f1d6592726561e8072389377e)`(const char * source,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` | Create a shader from a source string and shader type.
`public inline `[`sShader`](#structs_shader)` `[`loadShader`](#struct_graphics_module_1a31e7e5f097145854b41ba2202ab86cd5)`(const char * path,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` | Create a shader from a file path and shader type.
`public inline  explicit `[`GraphicsModule`](#struct_graphics_module_1a4efb7bf742a8d87e5cc043716c310657)`(const char * dynlib)` | Construct a graphics module from a dynamic library.

## Members

#### `public `[`sWindow`](#structs_window)` * `[`win`](#struct_graphics_module_1a64839384fd14c416875122e01dc37c28) <a id="struct_graphics_module_1a64839384fd14c416875122e01dc37c28"></a>

#### `public inline void `[`init`](#struct_graphics_module_1a72054b7c7196a3d2e978a1656451b7ff)`(`[`sWindow`](#structs_window)` * win)` <a id="struct_graphics_module_1a72054b7c7196a3d2e978a1656451b7ff"></a>

Initialize the graphics module.

This function initializes the graphics module with the specified window.

#### Parameters
* `win` The window to initialize the graphics module with.

#### `public inline void `[`setClearColor`](#struct_graphics_module_1a88a229195b716d94ab78293793d8d674)`(float r,float g,float b,float a)` <a id="struct_graphics_module_1a88a229195b716d94ab78293793d8d674"></a>

Set the clear color for the graphics module.

This function sets the clear color for the graphics module. This color is used to clear the screen before rendering.

#### Parameters
* `r` The red component of the clear color (0-1). 

* `g` The green component of the clear color (0-1). 

* `b` The blue component of the clear color (0-1). 

* `a` The alpha component of the clear color (0-1).

#### `public inline void `[`clear`](#struct_graphics_module_1aac7df8fed5ff4e0e08fe3b00d5dc9367)`()` <a id="struct_graphics_module_1aac7df8fed5ff4e0e08fe3b00d5dc9367"></a>

Clear the screen.

This function clears the screen with the clear color set by setClearColor. This MUST be called before rendering a new frame.

This function must be called before rendering a new frame because it does more than just clearing the screen. It also sets up the viewport and other graphics state (on some platforms).

#### `public inline void `[`present`](#struct_graphics_module_1a75745dcb3eea40c69ccb76436af9dfd2)`()` <a id="struct_graphics_module_1a75745dcb3eea40c69ccb76436af9dfd2"></a>

Present the rendered frame to the screen.

This function presents the rendered frame to the screen. This is typically called at the end of the rendering loop to display the rendered frame. This function does not swap buffers, it just presents the frame. The actual buffer swap is done by the windowing system.

This function must be called at the end of the rendering loop to display the rendered frame (on some platforms).

#### `public inline void `[`setScissor`](#struct_graphics_module_1a4a1a7bfac82e8d1386e42715fb3faf65)`(int x,int y,int width,int height)` <a id="struct_graphics_module_1a4a1a7bfac82e8d1386e42715fb3faf65"></a>

Set the scissor rectangle for rendering.

This function sets the scissor rectangle for rendering. The scissor rectangle is used to limit the area of the screen that is rendered to. This is useful for rendering to a specific area of the screen or for rendering to a texture.

#### Parameters
* `x` The x coordinate of the scissor rectangle. 

* `y` The y coordinate of the scissor rectangle. 

* `width` The width of the scissor rectangle. 

* `height` The height of the scissor rectangle.

#### `public inline void `[`enableScissor`](#struct_graphics_module_1a3ab028b9d6531ed4625089aeff8a90b3)`()` <a id="struct_graphics_module_1a3ab028b9d6531ed4625089aeff8a90b3"></a>

Enable the scissor rectangle for rendering.

This function enables the scissor rectangle for rendering. When the scissor rectangle is enabled, only the area inside the rectangle is rendered to.

#### `public inline void `[`disableScissor`](#struct_graphics_module_1a14ccfafb0a35746d6fa545465b3ce8e8)`()` <a id="struct_graphics_module_1a14ccfafb0a35746d6fa545465b3ce8e8"></a>

Disable the scissor rectangle for rendering.

This function disables the scissor rectangle for rendering. When the scissor rectangle is disabled, the entire screen is rendered to.

#### `public inline void `[`useShaderProgram`](#struct_graphics_module_1ae91c58bf252479f295203992abc89544)`(`[`sShaderProgram`](#structs_shader_program)` program)` <a id="struct_graphics_module_1ae91c58bf252479f295203992abc89544"></a>

Use a shader program for rendering.

This function uses the specified shader program for rendering. This must be called before rendering a mesh to use the shader program.

#### Parameters
* `program` The shader program to use for rendering. 

This function must be called before rendering a mesh to use the shader program.

#### `public inline void `[`drawMesh`](#struct_graphics_module_1a64d05631f87ad6b1bcc2819867531dd7)`(`[`sMesh`](#structs_mesh)` mesh)` <a id="struct_graphics_module_1a64d05631f87ad6b1bcc2819867531dd7"></a>

Draw a mesh to the screen.

This function draws the specified mesh to the screen. This must be called after using a shader program to render the mesh.

#### Parameters
* `mesh` The mesh to draw to the screen.

#### `public inline void `[`freeMesh`](#struct_graphics_module_1a68fb50ee03efd2df6ec267590ab53484)`(`[`sMesh`](#structs_mesh)` mesh)` <a id="struct_graphics_module_1a68fb50ee03efd2df6ec267590ab53484"></a>

Free a mesh.

This function frees the specified mesh. This should be called when the mesh is no longer needed to free the resources used by the mesh.

#### Parameters
* `mesh` The mesh to free.

#### `public inline void `[`freeShader`](#struct_graphics_module_1a0fc404fb66e2655dd396eb0f525de5b2)`(`[`sShader`](#structs_shader)` shader)` <a id="struct_graphics_module_1a0fc404fb66e2655dd396eb0f525de5b2"></a>

Free a shader.

This function frees the specified shader. This should be called when the shader is no longer needed to free the resources used by the shader.

#### Parameters
* `shader` The shader to free.

#### `public inline void `[`freeShaderProgram`](#struct_graphics_module_1af5ca2086152703a32c69414b11e6cc57)`(`[`sShaderProgram`](#structs_shader_program)` program)` <a id="struct_graphics_module_1af5ca2086152703a32c69414b11e6cc57"></a>

Free a shader program.

This function frees the specified shader program. This should be called when the shader program is no longer needed to free the resources used by the shader program.

#### `public inline void `[`freeUniforms`](#struct_graphics_module_1a3f4db4305879aaf22e35fa5c58dfd968)`(`[`sUniforms`](#structs_uniforms)` uniforms)` <a id="struct_graphics_module_1a3f4db4305879aaf22e35fa5c58dfd968"></a>

Free a uniforms object.

This function frees the specified uniforms object. This should be called when the uniforms object is no longer needed to free the resources used by the uniforms object.

#### `public inline void `[`freeTexture`](#struct_graphics_module_1a809f3292e3f94c6702dbb2020143f38d)`(`[`sTexture`](#structs_texture)` texture)` <a id="struct_graphics_module_1a809f3292e3f94c6702dbb2020143f38d"></a>

Free a texture.

This function frees the specified texture. This should be called when the texture is no longer needed to free the resources used by the texture.

#### `public inline void `[`destroy`](#struct_graphics_module_1a472b7333e889e2c0510512a1a41835d5)`()` <a id="struct_graphics_module_1a472b7333e889e2c0510512a1a41835d5"></a>

Destroy the graphics module.

This function destroys the graphics module and frees all resources used by the module.

#### `public inline `[`sTexture`](#structs_texture)` `[`createTexture`](#struct_graphics_module_1a2bdacd3c01063fd53d832f3a29b26477)`(`[`sTextureDefinition`](#structs_texture_definition)` def)` <a id="struct_graphics_module_1a2bdacd3c01063fd53d832f3a29b26477"></a>

Create a texture from a texture definition.

This function creates a texture from the specified texture definition.

#### Parameters
* `def` The texture definition to create the texture from. 

#### Returns
The created texture.

#### `public inline `[`sUniforms`](#structs_uniforms)` `[`createUniforms`](#struct_graphics_module_1a2f08045dbfe589f985952d13f82fd284)`(`[`sShaderProgram`](#structs_shader_program)` program,`[`sUniformDefinition`](#structs_uniform_definition)` def)` <a id="struct_graphics_module_1a2f08045dbfe589f985952d13f82fd284"></a>

Create a uniforms object from a shader program and a uniform definition.

This function creates a uniforms object from the specified shader program and uniform definition. This is used to set and get uniforms in a shader program.

#### Parameters
* `program` The shader program to create the uniforms object for. 

* `def` The uniform definition to create the uniforms object from. 

#### Returns
The created uniforms object.

#### `public inline void `[`setUniforms`](#struct_graphics_module_1a51fdd18499e304a05d7ae1886b868a4f)`(`[`sUniforms`](#structs_uniforms)` uniforms,void * data)` <a id="struct_graphics_module_1a51fdd18499e304a05d7ae1886b868a4f"></a>

Set the uniforms for a shader program.

This function sets the uniforms for the specified shader program. This must be called after creating a uniforms object to set the uniforms in the shader program.

#### Parameters
* `uniforms` The uniforms object to set the uniforms for. 

* `data` The data to set the uniforms to. This should be a pointer to the data that matches the uniform definition. 

This function must be called after creating a uniforms object to set the uniforms in the shader program. 

This void* data must be the same size as the uniform definition. This is not checked, so it is up to the user to ensure that the data is the correct size.

#### `public inline void `[`useTexture`](#struct_graphics_module_1a0f53efb3be6f4f0d50a3a5355df62d78)`(`[`sShaderProgram`](#structs_shader_program)` program,`[`sTexture`](#structs_texture)` texture,const char * name)` <a id="struct_graphics_module_1a0f53efb3be6f4f0d50a3a5355df62d78"></a>

Use a texture in a shader program.

This function uses the specified texture in the specified shader program. This must be called after using a shader program to use the texture in the shader program.

#### Parameters
* `program` The shader program to use the texture in. 

* `texture` The texture to use in the shader program. 

* `name` The name of the texture in the shader program.

#### `public inline const char * `[`getShaderType`](#struct_graphics_module_1abdb3a71f5aa4c1baa783a4d8cf57a9ed)`()` <a id="struct_graphics_module_1abdb3a71f5aa4c1baa783a4d8cf57a9ed"></a>

Get the shader type as a string.

This function returns the shader language format as a string. This is used to determine the shader language required for the shader compilation module.

#### `public inline `[`sMesh`](#structs_mesh)` `[`createMesh`](#struct_graphics_module_1a2080f1c1f12e7fe2b86045e099820390)`(`[`sShader`](#structs_shader)` vertexShader,void * vertices,size_t vertexCount,`[`sIndex`](#gfx_2module_8h_1aeec0f5b0bda581fc5280ed5a264d3da9)` * indices,size_t indexCount)` <a id="struct_graphics_module_1a2080f1c1f12e7fe2b86045e099820390"></a>

Create a mesh from a vertex shader, vertices, and indices.

This function creates a mesh from the specified vertex shader, vertices, and indices. This is used to create a mesh that can be rendered to the screen.

#### Parameters
* `vertexShader` The vertex shader to use for the mesh. 

* `vertices` The vertices to use for the mesh. This should be a pointer to the vertex data. 

* `vertexCount` The number of vertices in the mesh. 

* `indices` The indices to use for the mesh. This should be a pointer to the index data. 

* `indexCount` The number of indices in the mesh. 

#### Returns
The created mesh.

#### `public inline `[`sShaderProgram`](#structs_shader_program)` `[`createShaderProgram`](#struct_graphics_module_1a1291b5f0b13b4883ed273b2f41501bed)`(`[`sShader`](#structs_shader)` * shaders,size_t count)` <a id="struct_graphics_module_1a1291b5f0b13b4883ed273b2f41501bed"></a>

Create a shader program from an array of shaders.

This function creates a shader program from the specified array of shaders. This is used to create a shader program that can be used to render meshes.

#### Parameters
* `shaders` The array of shaders to use for the shader program. 

* `count` The number of shaders in the array. 

#### Returns
The created shader program.

#### `public inline `[`sShaderProgram`](#structs_shader_program)` `[`createShaderProgram`](#struct_graphics_module_1a5364ee0b194d3e4c440c9f3fd79a0cec)`(std::initializer_list< `[`sShader`](#structs_shader)` > shaders)` <a id="struct_graphics_module_1a5364ee0b194d3e4c440c9f3fd79a0cec"></a>

Create a shader program from an initializer list of shaders.

This function creates a shader program from the specified initializer list of shaders. This is used to create a shader program that can be used to render meshes.

#### Parameters
* `shaders` The initializer list of shaders to use for the shader program. 

#### Returns
The created shader program.

#### `public inline `[`sVertexDefinition`](#structs_vertex_definition)` * `[`createVertexDefinition`](#struct_graphics_module_1a0e870b8170deb3edf109c555d621a059)`(int * elements,size_t count)` <a id="struct_graphics_module_1a0e870b8170deb3edf109c555d621a059"></a>

Create a vertex definition from an array of elements.

This function creates a vertex definition from the specified array of elements. This is used to define the layout of a vertex in a vertex buffer.

#### Parameters
* `elements` The array of elements to use for the vertex definition. This should be a pointer to the element data. 

* `count` The number of elements in the array. 

#### Returns
The created vertex definition.

#### `public inline `[`sVertexDefinition`](#structs_vertex_definition)` * `[`createVertexDefinition`](#struct_graphics_module_1abcd20d04203631a0165af2d7a7ea6791)`(std::initializer_list< int > elements)` <a id="struct_graphics_module_1abcd20d04203631a0165af2d7a7ea6791"></a>

Create a vertex definition from an initializer list of elements.

This function creates a vertex definition from the specified initializer list of elements. This is used to define the layout of a vertex in a vertex buffer.

#### Parameters
* `elements` The initializer list of elements to use for the vertex definition. 

#### Returns
The created vertex definition.

#### `public inline void `[`freeVertexDefinition`](#struct_graphics_module_1a5edbde39320040e354485c88d17fcba8)`(`[`sVertexDefinition`](#structs_vertex_definition)` * def)` <a id="struct_graphics_module_1a5edbde39320040e354485c88d17fcba8"></a>

Free a vertex definition.

This function frees the specified vertex definition. This should be called when the vertex definition is no longer needed to free the resources used by the vertex definition.

#### Parameters
* `def` The vertex definition to free.

#### `public inline `[`sShader`](#structs_shader)` `[`createShader`](#struct_graphics_module_1ab53c3f4f1d6592726561e8072389377e)`(const char * source,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` <a id="struct_graphics_module_1ab53c3f4f1d6592726561e8072389377e"></a>

Create a shader from a source string and shader type.

This function creates a shader from the specified source string and shader type, and a vertex definition if the shader is a vertex shader. This is used to create a shader that can be used in a shader program.

#### Parameters
* `source` The source string to use for the shader. This should be a pointer to the source string. 

* `type` The type of the shader to create. This should be one of the shader types defined in sShaderType. 

* `vertDef` The vertex definition to use for the shader. This should be a pointer to the vertex definition. This is only used if the shader is a vertex shader. 

#### Returns
The created shader.

#### `public inline `[`sShader`](#structs_shader)` `[`loadShader`](#struct_graphics_module_1a31e7e5f097145854b41ba2202ab86cd5)`(const char * path,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` <a id="struct_graphics_module_1a31e7e5f097145854b41ba2202ab86cd5"></a>

Create a shader from a file path and shader type.

This function creates a shader from the specified file path and shader type, and a vertex definition if the shader is a vertex shader. This is used to create a shader that can be used in a shader program.

#### Parameters
* `path` The file path to the shader source file. This should be a pointer to the file path string. 

* `type` The type of the shader to create. This should be one of the shader types defined in sShaderType. 

* `vertDef` The vertex definition to use for the shader. This should be a pointer to the vertex definition. This is only used if the shader is a vertex shader. 

#### Returns
The created shader.

#### `public inline  explicit `[`GraphicsModule`](#struct_graphics_module_1a4efb7bf742a8d87e5cc043716c310657)`(const char * dynlib)` <a id="struct_graphics_module_1a4efb7bf742a8d87e5cc043716c310657"></a>

Construct a graphics module from a dynamic library.

This function constructs a graphics module from the specified dynamic library. This is used to load the graphics module at runtime.

#### Parameters
* `dynlib` The implementation string to use for the graphics module.

# struct `mat4.__unnamed13__` <a id="structmat4_8____unnamed13____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vec4 `[`x`](#structmat4_8____unnamed13_____1a9dd4e461268c8034f5c8564e155c67a6) | 
`public vec4 `[`y`](#structmat4_8____unnamed13_____1a415290769594460e2e485922904f345d) | 
`public vec4 `[`z`](#structmat4_8____unnamed13_____1afbade9e36a3f36d3d676c1b808451dd7) | 
`public vec4 `[`w`](#structmat4_8____unnamed13_____1af1290186a5d0b1ceab27f4e77c0c5d68) | 

## Members

#### `public vec4 `[`x`](#structmat4_8____unnamed13_____1a9dd4e461268c8034f5c8564e155c67a6) <a id="structmat4_8____unnamed13_____1a9dd4e461268c8034f5c8564e155c67a6"></a>

#### `public vec4 `[`y`](#structmat4_8____unnamed13_____1a415290769594460e2e485922904f345d) <a id="structmat4_8____unnamed13_____1a415290769594460e2e485922904f345d"></a>

#### `public vec4 `[`z`](#structmat4_8____unnamed13_____1afbade9e36a3f36d3d676c1b808451dd7) <a id="structmat4_8____unnamed13_____1afbade9e36a3f36d3d676c1b808451dd7"></a>

#### `public vec4 `[`w`](#structmat4_8____unnamed13_____1af1290186a5d0b1ceab27f4e77c0c5d68) <a id="structmat4_8____unnamed13_____1af1290186a5d0b1ceab27f4e77c0c5d68"></a>

# struct `Module` <a id="struct_module"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public DynamicLibrary `[`lib`](#struct_module_1aeb415182f1cd3d4536fa7bde81fec1ec) | 
`public inline  explicit `[`Module`](#struct_module_1abaed7f29287dabeebc1d20de1c5ada14)`(const char * path,const char * ident)` | 

## Members

#### `public DynamicLibrary `[`lib`](#struct_module_1aeb415182f1cd3d4536fa7bde81fec1ec) <a id="struct_module_1aeb415182f1cd3d4536fa7bde81fec1ec"></a>

#### `public inline  explicit `[`Module`](#struct_module_1abaed7f29287dabeebc1d20de1c5ada14)`(const char * path,const char * ident)` <a id="struct_module_1abaed7f29287dabeebc1d20de1c5ada14"></a>

# struct `sAudioClip` <a id="structs_audio_clip"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_audio_clip_1a5b09091cf9d5c3db03cb8fa4a3538337) | 

## Members

#### `public void * `[`internal`](#structs_audio_clip_1a5b09091cf9d5c3db03cb8fa4a3538337) <a id="structs_audio_clip_1a5b09091cf9d5c3db03cb8fa4a3538337"></a>

# struct `sAudioSource` <a id="structs_audio_source"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_audio_source_1a399802c1b4eda4fca4bd377fa44430be) | 
`public float `[`posX`](#structs_audio_source_1a548a4f081263fe3a412b57dad42844c1) | 
`public float `[`posY`](#structs_audio_source_1a64bd05a1e107f13c7f8233eacaff789a) | 
`public float `[`posZ`](#structs_audio_source_1ab4e14f96dde5d8370a4aa679bc8370fb) | 
`public float `[`velX`](#structs_audio_source_1aae9f5659acf89aeed42019ea7e770015) | 
`public float `[`velY`](#structs_audio_source_1a1e3e4c2cf026980fd2fc85aa058b9711) | 
`public float `[`velZ`](#structs_audio_source_1a7d84a1a70ab7068e5230461c3a6f29fc) | 

## Members

#### `public void * `[`internal`](#structs_audio_source_1a399802c1b4eda4fca4bd377fa44430be) <a id="structs_audio_source_1a399802c1b4eda4fca4bd377fa44430be"></a>

#### `public float `[`posX`](#structs_audio_source_1a548a4f081263fe3a412b57dad42844c1) <a id="structs_audio_source_1a548a4f081263fe3a412b57dad42844c1"></a>

#### `public float `[`posY`](#structs_audio_source_1a64bd05a1e107f13c7f8233eacaff789a) <a id="structs_audio_source_1a64bd05a1e107f13c7f8233eacaff789a"></a>

#### `public float `[`posZ`](#structs_audio_source_1ab4e14f96dde5d8370a4aa679bc8370fb) <a id="structs_audio_source_1ab4e14f96dde5d8370a4aa679bc8370fb"></a>

#### `public float `[`velX`](#structs_audio_source_1aae9f5659acf89aeed42019ea7e770015) <a id="structs_audio_source_1aae9f5659acf89aeed42019ea7e770015"></a>

#### `public float `[`velY`](#structs_audio_source_1a1e3e4c2cf026980fd2fc85aa058b9711) <a id="structs_audio_source_1a1e3e4c2cf026980fd2fc85aa058b9711"></a>

#### `public float `[`velZ`](#structs_audio_source_1a7d84a1a70ab7068e5230461c3a6f29fc) <a id="structs_audio_source_1a7d84a1a70ab7068e5230461c3a6f29fc"></a>

# struct `sCamera` <a id="structs_camera"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vec3 `[`pos`](#structs_camera_1a4b94bb74bd24be1efc4e160435ae4b3a) | 
`public vec3 `[`up`](#structs_camera_1ac1844af4a3e0292aab1c84022493a1a5) | 
`public vec3 `[`forward`](#structs_camera_1ac0a76945dff826b387ca385cc15b4ec4) | 
`public float `[`yaw`](#structs_camera_1a338014b5eba9a892111da9b29775f012) | 
`public float `[`pitch`](#structs_camera_1a72641be2dd535a719d363dae63c2807d) | 
`public inline vec3 `[`right`](#structs_camera_1a327743eaabccc477b78e7cb6e83ea06c)`()` | 
`public inline vec3 `[`left`](#structs_camera_1ad5700423000b9b58b92477a1d17d78f1)`()` | 
`public inline vec3 `[`back`](#structs_camera_1af756a5ae5b9ad6c2061871d8e18415ec)`()` | 
`public inline vec3 `[`down`](#structs_camera_1a52a4247fffb271d5bf299a6f312f073a)`()` | 
`public inline vec3 `[`right`](#structs_camera_1a9fb08058ab74f1f1b0430b34421d741c)`(vec3 forward)` | 
`public inline vec3 `[`left`](#structs_camera_1ae556d8c13622c8f48f076ccc360d8752)`(vec3 forward)` | 
`public inline vec3 `[`back`](#structs_camera_1a026be479af001be2a70ecbd7f7fc203f)`(vec3 forward)` | 
`public inline vec3 `[`down`](#structs_camera_1a70185b7a8aa602d6dd07cec3ad911d65)`(vec3 up)` | 

## Members

#### `public vec3 `[`pos`](#structs_camera_1a4b94bb74bd24be1efc4e160435ae4b3a) <a id="structs_camera_1a4b94bb74bd24be1efc4e160435ae4b3a"></a>

#### `public vec3 `[`up`](#structs_camera_1ac1844af4a3e0292aab1c84022493a1a5) <a id="structs_camera_1ac1844af4a3e0292aab1c84022493a1a5"></a>

#### `public vec3 `[`forward`](#structs_camera_1ac0a76945dff826b387ca385cc15b4ec4) <a id="structs_camera_1ac0a76945dff826b387ca385cc15b4ec4"></a>

#### `public float `[`yaw`](#structs_camera_1a338014b5eba9a892111da9b29775f012) <a id="structs_camera_1a338014b5eba9a892111da9b29775f012"></a>

#### `public float `[`pitch`](#structs_camera_1a72641be2dd535a719d363dae63c2807d) <a id="structs_camera_1a72641be2dd535a719d363dae63c2807d"></a>

#### `public inline vec3 `[`right`](#structs_camera_1a327743eaabccc477b78e7cb6e83ea06c)`()` <a id="structs_camera_1a327743eaabccc477b78e7cb6e83ea06c"></a>

#### `public inline vec3 `[`left`](#structs_camera_1ad5700423000b9b58b92477a1d17d78f1)`()` <a id="structs_camera_1ad5700423000b9b58b92477a1d17d78f1"></a>

#### `public inline vec3 `[`back`](#structs_camera_1af756a5ae5b9ad6c2061871d8e18415ec)`()` <a id="structs_camera_1af756a5ae5b9ad6c2061871d8e18415ec"></a>

#### `public inline vec3 `[`down`](#structs_camera_1a52a4247fffb271d5bf299a6f312f073a)`()` <a id="structs_camera_1a52a4247fffb271d5bf299a6f312f073a"></a>

#### `public inline vec3 `[`right`](#structs_camera_1a9fb08058ab74f1f1b0430b34421d741c)`(vec3 forward)` <a id="structs_camera_1a9fb08058ab74f1f1b0430b34421d741c"></a>

#### `public inline vec3 `[`left`](#structs_camera_1ae556d8c13622c8f48f076ccc360d8752)`(vec3 forward)` <a id="structs_camera_1ae556d8c13622c8f48f076ccc360d8752"></a>

#### `public inline vec3 `[`back`](#structs_camera_1a026be479af001be2a70ecbd7f7fc203f)`(vec3 forward)` <a id="structs_camera_1a026be479af001be2a70ecbd7f7fc203f"></a>

#### `public inline vec3 `[`down`](#structs_camera_1a70185b7a8aa602d6dd07cec3ad911d65)`(vec3 up)` <a id="structs_camera_1a70185b7a8aa602d6dd07cec3ad911d65"></a>

# struct `Script` <a id="struct_script"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#struct_script_1aa25d913669150198b28cbe7b6b16c20e) | 
`public ScriptInit `[`init`](#struct_script_1af99bc626ac8e85cd9aa44639844cd5c8) | 
`public ScriptUpdate `[`update`](#struct_script_1af9c93d3b5b4ca0fac86af4ce54fdbda4) | 

## Members

#### `public void * `[`internal`](#struct_script_1aa25d913669150198b28cbe7b6b16c20e) <a id="struct_script_1aa25d913669150198b28cbe7b6b16c20e"></a>

#### `public ScriptInit `[`init`](#struct_script_1af99bc626ac8e85cd9aa44639844cd5c8) <a id="struct_script_1af99bc626ac8e85cd9aa44639844cd5c8"></a>

#### `public ScriptUpdate `[`update`](#struct_script_1af9c93d3b5b4ca0fac86af4ce54fdbda4) <a id="struct_script_1af9c93d3b5b4ca0fac86af4ce54fdbda4"></a>

# struct `ScriptLoaderModule` <a id="struct_script_loader_module"></a>

```
struct ScriptLoaderModule
  : public Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public scrload::ScriptLoader `[`loadScript`](#struct_script_loader_module_1ab01a5bbd007ba28d5c6f620b845f91af) | 
`public scrload::ScriptCompiler `[`compileScript`](#struct_script_loader_module_1a47b2843891f2de166db87f016212d001) | 
`public scrload::MultiScriptCompiler `[`compileScripts`](#struct_script_loader_module_1a23b1fdbcb7b6d4d787e9e0c0b340f966) | 
`public char * `[`inputExtension`](#struct_script_loader_module_1ad02154d7b54590bad9b8e547d19352f2) | 
`public char * `[`outputExtension`](#struct_script_loader_module_1a55634815c7c426abcecdeb9ebf1a41b4) | 
`public inline  explicit `[`ScriptLoaderModule`](#struct_script_loader_module_1a753f0382f59a621f3392d51daf6b0a99)`(const char * dynlib)` | 

## Members

#### `public scrload::ScriptLoader `[`loadScript`](#struct_script_loader_module_1ab01a5bbd007ba28d5c6f620b845f91af) <a id="struct_script_loader_module_1ab01a5bbd007ba28d5c6f620b845f91af"></a>

#### `public scrload::ScriptCompiler `[`compileScript`](#struct_script_loader_module_1a47b2843891f2de166db87f016212d001) <a id="struct_script_loader_module_1a47b2843891f2de166db87f016212d001"></a>

#### `public scrload::MultiScriptCompiler `[`compileScripts`](#struct_script_loader_module_1a23b1fdbcb7b6d4d787e9e0c0b340f966) <a id="struct_script_loader_module_1a23b1fdbcb7b6d4d787e9e0c0b340f966"></a>

#### `public char * `[`inputExtension`](#struct_script_loader_module_1ad02154d7b54590bad9b8e547d19352f2) <a id="struct_script_loader_module_1ad02154d7b54590bad9b8e547d19352f2"></a>

#### `public char * `[`outputExtension`](#struct_script_loader_module_1a55634815c7c426abcecdeb9ebf1a41b4) <a id="struct_script_loader_module_1a55634815c7c426abcecdeb9ebf1a41b4"></a>

#### `public inline  explicit `[`ScriptLoaderModule`](#struct_script_loader_module_1a753f0382f59a621f3392d51daf6b0a99)`(const char * dynlib)` <a id="struct_script_loader_module_1a753f0382f59a621f3392d51daf6b0a99"></a>

# struct `sFont` <a id="structs_font"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_font_1a60be07a314f3c87856079a3922836f32) | 
`public int `[`size`](#structs_font_1a3c7c805f5b49974ac4446a76c20449b6) | 

## Members

#### `public void * `[`internal`](#structs_font_1a60be07a314f3c87856079a3922836f32) <a id="structs_font_1a60be07a314f3c87856079a3922836f32"></a>

#### `public int `[`size`](#structs_font_1a3c7c805f5b49974ac4446a76c20449b6) <a id="structs_font_1a3c7c805f5b49974ac4446a76c20449b6"></a>

# struct `ShaderModule` <a id="struct_shader_module"></a>

```
struct ShaderModule
  : public Module
```  

[ShaderModule](#struct_shader_module) is a module that provides functionality for compiling and creating shaders.

The [ShaderModule](#struct_shader_module) is a dynamic library that provides functionality for compiling and creating shaders. It uses the [GraphicsModule](#struct_graphics_module) to compile the shader. The shader is compiled from the file at the given path.

This is helpful because it allows us to write a single shader in a single language, instead of having to rewrite it to support each graphics API.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline `[`sShader`](#structs_shader)` `[`compile`](#struct_shader_module_1a4820c0e1c54fd78e687fd292c478c9ac)`(`[`GraphicsModule`](#struct_graphics_module)` * gfxm,const char * path,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` | Compiles a shader from a file.
`public inline `[`sShader`](#structs_shader)` `[`createShader`](#struct_shader_module_1a79a43cd49d1fe6e25f246c3b284b62a0)`(`[`GraphicsModule`](#struct_graphics_module)` * gfxm,const char * data,size_t len,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` | Creates a shader from a string.
`public inline  explicit `[`ShaderModule`](#struct_shader_module_1a003135da7d77e951b291b8431ec6546f)`(const char * dynlib,const char * dynp2)` | Constructs a [ShaderModule](#struct_shader_module) object.

## Members

#### `public inline `[`sShader`](#structs_shader)` `[`compile`](#struct_shader_module_1a4820c0e1c54fd78e687fd292c478c9ac)`(`[`GraphicsModule`](#struct_graphics_module)` * gfxm,const char * path,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` <a id="struct_shader_module_1a4820c0e1c54fd78e687fd292c478c9ac"></a>

Compiles a shader from a file.

Uses the [GraphicsModule](#struct_graphics_module) to compile the shader. The shader is compiled from the file at the given path.

#### Parameters
* `gfxm` The [GraphicsModule](#struct_graphics_module) to use for compilation. 

* `path` The path to the shader file. 

* `type` The type of shader to compile. 

* `vertDef` The vertex definition to use for the shader. Only required for vertex shaders. 

#### Returns
The compiled shader. 

This function will compile from any type of shader file (glsl, hlsl, spsl, etc.) depending on which implementation is used. 

**See also**: [sShaderType](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)

#### `public inline `[`sShader`](#structs_shader)` `[`createShader`](#struct_shader_module_1a79a43cd49d1fe6e25f246c3b284b62a0)`(`[`GraphicsModule`](#struct_graphics_module)` * gfxm,const char * data,size_t len,`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` type,`[`sVertexDefinition`](#structs_vertex_definition)` * vertDef)` <a id="struct_shader_module_1a79a43cd49d1fe6e25f246c3b284b62a0"></a>

Creates a shader from a string.

Uses the [GraphicsModule](#struct_graphics_module) to create the shader. The shader is created from the string data.

#### Parameters
* `gfxm` The [GraphicsModule](#struct_graphics_module) to use for creating the shader. 

* `data` The shader data. 

* `len` The length of the shader data. 

* `type` The type of shader to create. 

* `vertDef` The vertex definition to use for the shader. Only required for vertex shaders. 

#### Returns
The created shader. 

**See also**: [ShaderModule::compile](#struct_shader_module_1a4820c0e1c54fd78e687fd292c478c9ac)

#### `public inline  explicit `[`ShaderModule`](#struct_shader_module_1a003135da7d77e951b291b8431ec6546f)`(const char * dynlib,const char * dynp2)` <a id="struct_shader_module_1a003135da7d77e951b291b8431ec6546f"></a>

Constructs a [ShaderModule](#struct_shader_module) object.

The [ShaderModule](#struct_shader_module) is a dynamic library that provides functionality for compiling and creating shaders. It uses the [GraphicsModule](#struct_graphics_module) to compile the shader. The shader is compiled from the file at the given path.

#### Parameters
* `dynlib` The dynamic library implementation to use. 

* `dynp2` The output type. This is given as a string, normally straight from the graphics module's getShaderType function. 

**See also**: [GraphicsModule::getShaderType](#struct_graphics_module_1abdb3a71f5aa4c1baa783a4d8cf57a9ed)

> Todo: update this once the graphics module has docs

# struct `sMesh` <a id="structs_mesh"></a>

Mesh structure.

This structure represents a mesh that can be used in a shader program. Implementation defined elsewhere. It is used to create and draw meshes to the screen.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_mesh_1a5eea482db47731c601ec10514b5052a6) | 
`public `[`GraphicsModule`](#struct_graphics_module)` * `[`creator`](#structs_mesh_1a10d729e60d51c4dd812519dc55a5226a) | 

## Members

#### `public void * `[`internal`](#structs_mesh_1a5eea482db47731c601ec10514b5052a6) <a id="structs_mesh_1a5eea482db47731c601ec10514b5052a6"></a>

#### `public `[`GraphicsModule`](#struct_graphics_module)` * `[`creator`](#structs_mesh_1a10d729e60d51c4dd812519dc55a5226a) <a id="structs_mesh_1a10d729e60d51c4dd812519dc55a5226a"></a>

# struct `sModelTransform` <a id="structs_model_transform"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vec3 `[`pos`](#structs_model_transform_1aac335e8a460119b02ade04cb145ccb87) | 
`public vec3 `[`sca`](#structs_model_transform_1a5cf86854bf9d3f34b4fa5f9ad7ef2bcb) | 
`public vec3 `[`rot`](#structs_model_transform_1a7aa59462936ba2c55431ff243f693f23) | 
`public vec3 `[`lastPos`](#structs_model_transform_1a3f8b10396dd85575a1e50351184123b3) | 
`public vec3 `[`lastSca`](#structs_model_transform_1aac6f3894b99298df72a7ec2c6f258282) | 
`public vec3 `[`lastRot`](#structs_model_transform_1a65904bb9e0b6ddc0423163fb3b6107d7) | 
`public mat4 `[`internal_matrix`](#structs_model_transform_1ab447224c6f8735bb1b2f5f638f35e17f) | 
`public inline mat4 `[`matrix`](#structs_model_transform_1a439db170aa197a5c9a714190ff182d46)`()` | 

## Members

#### `public vec3 `[`pos`](#structs_model_transform_1aac335e8a460119b02ade04cb145ccb87) <a id="structs_model_transform_1aac335e8a460119b02ade04cb145ccb87"></a>

#### `public vec3 `[`sca`](#structs_model_transform_1a5cf86854bf9d3f34b4fa5f9ad7ef2bcb) <a id="structs_model_transform_1a5cf86854bf9d3f34b4fa5f9ad7ef2bcb"></a>

#### `public vec3 `[`rot`](#structs_model_transform_1a7aa59462936ba2c55431ff243f693f23) <a id="structs_model_transform_1a7aa59462936ba2c55431ff243f693f23"></a>

#### `public vec3 `[`lastPos`](#structs_model_transform_1a3f8b10396dd85575a1e50351184123b3) <a id="structs_model_transform_1a3f8b10396dd85575a1e50351184123b3"></a>

#### `public vec3 `[`lastSca`](#structs_model_transform_1aac6f3894b99298df72a7ec2c6f258282) <a id="structs_model_transform_1aac6f3894b99298df72a7ec2c6f258282"></a>

#### `public vec3 `[`lastRot`](#structs_model_transform_1a65904bb9e0b6ddc0423163fb3b6107d7) <a id="structs_model_transform_1a65904bb9e0b6ddc0423163fb3b6107d7"></a>

#### `public mat4 `[`internal_matrix`](#structs_model_transform_1ab447224c6f8735bb1b2f5f638f35e17f) <a id="structs_model_transform_1ab447224c6f8735bb1b2f5f638f35e17f"></a>

#### `public inline mat4 `[`matrix`](#structs_model_transform_1a439db170aa197a5c9a714190ff182d46)`()` <a id="structs_model_transform_1a439db170aa197a5c9a714190ff182d46"></a>

# struct `sShader` <a id="structs_shader"></a>

Shader structure.

This structure represents a shader that can be used in a shader program. Implementation defined elsewhere. It is a single shader (not a shader program) and is used to create shader programs.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_shader_1a65c55d2d481941e14db4ccee355d411f) | 

## Members

#### `public void * `[`internal`](#structs_shader_1a65c55d2d481941e14db4ccee355d411f) <a id="structs_shader_1a65c55d2d481941e14db4ccee355d411f"></a>

# struct `sShaderProgram` <a id="structs_shader_program"></a>

Shader program structure.

This structure represents a shader program that can be used to render meshes. Implementation defined elsewhere. It is a collection of shaders that are used together to render a mesh.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_shader_program_1a2f2cc0c04aedb1497299bc2e3e79641c) | 
`public `[`GraphicsModule`](#struct_graphics_module)` * `[`creator`](#structs_shader_program_1a4fea0cc5a2b5954e764cca58dd3bff26) | 

## Members

#### `public void * `[`internal`](#structs_shader_program_1a2f2cc0c04aedb1497299bc2e3e79641c) <a id="structs_shader_program_1a2f2cc0c04aedb1497299bc2e3e79641c"></a>

#### `public `[`GraphicsModule`](#struct_graphics_module)` * `[`creator`](#structs_shader_program_1a4fea0cc5a2b5954e764cca58dd3bff26) <a id="structs_shader_program_1a4fea0cc5a2b5954e764cca58dd3bff26"></a>

# struct `sText` <a id="structs_text"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_text_1afe4d59c0294d06a08a2255539351fee0) | 

## Members

#### `public void * `[`internal`](#structs_text_1afe4d59c0294d06a08a2255539351fee0) <a id="structs_text_1afe4d59c0294d06a08a2255539351fee0"></a>

# struct `sTexture` <a id="structs_texture"></a>

Texture structure.

This structure represents a texture that can be used in a shader program. Implementation defined elsewhere. It is used to create and use textures in a shader program.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_texture_1abb580940243f1d7fe2b20c9274bf448d) | 

## Members

#### `public void * `[`internal`](#structs_texture_1abb580940243f1d7fe2b20c9274bf448d) <a id="structs_texture_1abb580940243f1d7fe2b20c9274bf448d"></a>

# struct `sTextureDefinition` <a id="structs_texture_definition"></a>

Texture definition structure.

This structure defines a texture that can be used in a shader program. It contains the width, height, number of channels, and data of the texture. The data is a pointer to the raw texture data, which is an array of unsigned char values. This cannot be used directly, but is used to create a texture.

This is an intermediate structure and should be used only to create a texture.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public size_t `[`width`](#structs_texture_definition_1a6563d20b1246111b13c926eac88a1155) | 
`public size_t `[`height`](#structs_texture_definition_1af8afad0c466462bd5fed3deaa698f341) | 
`public size_t `[`channels`](#structs_texture_definition_1a760872fd8b6aad620b9b420c239bb482) | 
`public unsigned char * `[`data`](#structs_texture_definition_1a2f32c7ac9e6e811419198cb9f18a3d28) | 

## Members

#### `public size_t `[`width`](#structs_texture_definition_1a6563d20b1246111b13c926eac88a1155) <a id="structs_texture_definition_1a6563d20b1246111b13c926eac88a1155"></a>

#### `public size_t `[`height`](#structs_texture_definition_1af8afad0c466462bd5fed3deaa698f341) <a id="structs_texture_definition_1af8afad0c466462bd5fed3deaa698f341"></a>

#### `public size_t `[`channels`](#structs_texture_definition_1a760872fd8b6aad620b9b420c239bb482) <a id="structs_texture_definition_1a760872fd8b6aad620b9b420c239bb482"></a>

#### `public unsigned char * `[`data`](#structs_texture_definition_1a2f32c7ac9e6e811419198cb9f18a3d28) <a id="structs_texture_definition_1a2f32c7ac9e6e811419198cb9f18a3d28"></a>

# struct `sUniformDefinition` <a id="structs_uniform_definition"></a>

Uniform definition structure.

This structure defines a uniform definition, which is a collection of uniform elements. It contains an array of uniform elements and a count of the number of elements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`sUniformElement`](#structs_uniform_element)` * `[`elements`](#structs_uniform_definition_1a33e57330bc33dd214b0624cd11b5624e) | 
`public size_t `[`count`](#structs_uniform_definition_1a50de8f1a6258dd406203cc399143de09) | 
`public inline  `[`sUniformDefinition`](#structs_uniform_definition_1af85f36e434e17dfe05d6fb3e21c1c8b8)`(std::initializer_list< `[`sUniformElement`](#structs_uniform_element)` > elements)` | Construct a uniform definition from an initializer list of uniform elements.
`public inline size_t `[`size`](#structs_uniform_definition_1a96ca02cbeb0e9a03c19c50596d61ee8d)`()` | Get the size (in bytes) of the uniform definition.
`public inline  `[`sUniformDefinition`](#structs_uniform_definition_1a3388507dbf49e85d3eaeba62419523fc)`()` | Construct an empty uniform definition.
`public inline  `[`sUniformDefinition`](#structs_uniform_definition_1accb3656a2915c8936434d67198148b3c)`(`[`sUniformElement`](#structs_uniform_element)` * elements,size_t count)` | Construct a uniform definition from an array of uniform elements.

## Members

#### `public `[`sUniformElement`](#structs_uniform_element)` * `[`elements`](#structs_uniform_definition_1a33e57330bc33dd214b0624cd11b5624e) <a id="structs_uniform_definition_1a33e57330bc33dd214b0624cd11b5624e"></a>

#### `public size_t `[`count`](#structs_uniform_definition_1a50de8f1a6258dd406203cc399143de09) <a id="structs_uniform_definition_1a50de8f1a6258dd406203cc399143de09"></a>

#### `public inline  `[`sUniformDefinition`](#structs_uniform_definition_1af85f36e434e17dfe05d6fb3e21c1c8b8)`(std::initializer_list< `[`sUniformElement`](#structs_uniform_element)` > elements)` <a id="structs_uniform_definition_1af85f36e434e17dfe05d6fb3e21c1c8b8"></a>

Construct a uniform definition from an initializer list of uniform elements.

#### Parameters
* `elements` The initializer list of uniform elements.

#### `public inline size_t `[`size`](#structs_uniform_definition_1a96ca02cbeb0e9a03c19c50596d61ee8d)`()` <a id="structs_uniform_definition_1a96ca02cbeb0e9a03c19c50596d61ee8d"></a>

Get the size (in bytes) of the uniform definition.

#### Returns
The size of the uniform definition in bytes.

#### `public inline  `[`sUniformDefinition`](#structs_uniform_definition_1a3388507dbf49e85d3eaeba62419523fc)`()` <a id="structs_uniform_definition_1a3388507dbf49e85d3eaeba62419523fc"></a>

Construct an empty uniform definition.

#### `public inline  `[`sUniformDefinition`](#structs_uniform_definition_1accb3656a2915c8936434d67198148b3c)`(`[`sUniformElement`](#structs_uniform_element)` * elements,size_t count)` <a id="structs_uniform_definition_1accb3656a2915c8936434d67198148b3c"></a>

Construct a uniform definition from an array of uniform elements.

#### Parameters
* `elements` The array of uniform elements. 

* `count` The number of uniform elements.

# struct `sUniformElement` <a id="structs_uniform_element"></a>

Uniform element structure.

This structure defines a single uniform element in a shader program. It contains the shader type, name, type, and size of the uniform. The size is defined as countx * county, where countx is the number of components and county is the number of elements in the array.

> Todo: Add support for arrays like vec3[] and mat4[]. 

Test if uneven matrices work (e.g. mat3x4).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` `[`shaderType`](#structs_uniform_element_1a88c8dff962c30a58eefa880567a96a18) | 
`public const char * `[`name`](#structs_uniform_element_1acb9b1ff9b6db0a44da2a836393384e5a) | 
`public `[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` `[`type`](#structs_uniform_element_1a676b83f600fd9b52396f5536d500497a) | 
`public size_t `[`countx`](#structs_uniform_element_1ac092d3338c1c1f61c0201f89ad7a252c) | 
`public size_t `[`county`](#structs_uniform_element_1a8369bc212d7a8be69b4a702b61b63ee0) | 
`public inline  `[`sUniformElement`](#structs_uniform_element_1a8bc333f83e5866e4270e7df1a66b0f07)`(`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` shaderType,const char * name,`[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` type,size_t countx,size_t county)` | Construct a matrix uniform element.
`public inline  `[`sUniformElement`](#structs_uniform_element_1a3c097b87365c1a9dd7a44ef81f45f1de)`(`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` shaderType,const char * name,`[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` type,size_t countx)` | Construct a vector or scalar uniform element.

## Members

#### `public `[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` `[`shaderType`](#structs_uniform_element_1a88c8dff962c30a58eefa880567a96a18) <a id="structs_uniform_element_1a88c8dff962c30a58eefa880567a96a18"></a>

#### `public const char * `[`name`](#structs_uniform_element_1acb9b1ff9b6db0a44da2a836393384e5a) <a id="structs_uniform_element_1acb9b1ff9b6db0a44da2a836393384e5a"></a>

#### `public `[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` `[`type`](#structs_uniform_element_1a676b83f600fd9b52396f5536d500497a) <a id="structs_uniform_element_1a676b83f600fd9b52396f5536d500497a"></a>

#### `public size_t `[`countx`](#structs_uniform_element_1ac092d3338c1c1f61c0201f89ad7a252c) <a id="structs_uniform_element_1ac092d3338c1c1f61c0201f89ad7a252c"></a>

#### `public size_t `[`county`](#structs_uniform_element_1a8369bc212d7a8be69b4a702b61b63ee0) <a id="structs_uniform_element_1a8369bc212d7a8be69b4a702b61b63ee0"></a>

#### `public inline  `[`sUniformElement`](#structs_uniform_element_1a8bc333f83e5866e4270e7df1a66b0f07)`(`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` shaderType,const char * name,`[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` type,size_t countx,size_t county)` <a id="structs_uniform_element_1a8bc333f83e5866e4270e7df1a66b0f07"></a>

Construct a matrix uniform element.

#### Parameters
* `shaderType` The shader that this uniform belongs to. 

* `name` The name of the uniform in the shader. 

* `type` The type of the uniform. 

* `countx` The x size of the matrix (e.g. 4 for a mat4). 

* `county` The y size of the matrix (e.g. 4 for a mat4).

#### `public inline  `[`sUniformElement`](#structs_uniform_element_1a3c097b87365c1a9dd7a44ef81f45f1de)`(`[`sShaderType`](#gfx_2module_8h_1afb4c39d3e32b47474f8c0e18f26a4ecd)` shaderType,const char * name,`[`sUniformType`](#gfx_2module_8h_1ab36772a0596f81df1a2cbbd7d12aa809)` type,size_t countx)` <a id="structs_uniform_element_1a3c097b87365c1a9dd7a44ef81f45f1de"></a>

Construct a vector or scalar uniform element.

#### Parameters
* `shaderType` The shader that this uniform belongs to. 

* `name` The name of the uniform in the shader. 

* `type` The type of the uniform. 

* `countx` The size of the vector (e.g. 4 for a vec4).

# struct `sUniforms` <a id="structs_uniforms"></a>

Uniforms structure.

This structure represents a collection of uniforms that can be used in a shader program. Implementation defined elsewhere. It is used to set and get uniforms in a shader program.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * `[`internal`](#structs_uniforms_1aea8d6998a8d1a4a0f903132f0b70243f) | 

## Members

#### `public void * `[`internal`](#structs_uniforms_1aea8d6998a8d1a4a0f903132f0b70243f) <a id="structs_uniforms_1aea8d6998a8d1a4a0f903132f0b70243f"></a>

# struct `sVertexDefinition` <a id="structs_vertex_definition"></a>

A single abstract vertex definition.

This structure defines the layout of a vertex in a vertex buffer. It contains an array of integers that represent the number of components for each attribute in the vertex, and a count of the number of attributes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int * `[`elements`](#structs_vertex_definition_1af9299e7cca87d61283a420a55c35a416) | 
`public size_t `[`count`](#structs_vertex_definition_1a8dbc07228a77c5561b166b9e838f1625) | 

## Members

#### `public int * `[`elements`](#structs_vertex_definition_1af9299e7cca87d61283a420a55c35a416) <a id="structs_vertex_definition_1af9299e7cca87d61283a420a55c35a416"></a>

#### `public size_t `[`count`](#structs_vertex_definition_1a8dbc07228a77c5561b166b9e838f1625) <a id="structs_vertex_definition_1a8dbc07228a77c5561b166b9e838f1625"></a>

# struct `swAudio` <a id="structsw_audio"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string `[`path`](#structsw_audio_1a50997bb847df1f428fad5eec764ca685) | 
`public std::vector< uint8_t > `[`data`](#structsw_audio_1aee63ef0297efb28460b1d2647824344e) | 

## Members

#### `public std::string `[`path`](#structsw_audio_1a50997bb847df1f428fad5eec764ca685) <a id="structsw_audio_1a50997bb847df1f428fad5eec764ca685"></a>

#### `public std::vector< uint8_t > `[`data`](#structsw_audio_1aee63ef0297efb28460b1d2647824344e) <a id="structsw_audio_1aee63ef0297efb28460b1d2647824344e"></a>

# struct `swEtc` <a id="structsw_etc"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public enum swEtc::swEtcType `[`type`](#structsw_etc_1ae5f13945f5b22c817298757a1cb52dcc) | 
`enum `[`swEtcType`](#structsw_etc_1a809542eeac4b9d79098774fe3be0f08b) | 

## Members

#### `public enum swEtc::swEtcType `[`type`](#structsw_etc_1ae5f13945f5b22c817298757a1cb52dcc) <a id="structsw_etc_1ae5f13945f5b22c817298757a1cb52dcc"></a>

#### `enum `[`swEtcType`](#structsw_etc_1a809542eeac4b9d79098774fe3be0f08b) <a id="structsw_etc_1a809542eeac4b9d79098774fe3be0f08b"></a>

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
PRE_LOAD            | 
PRE_LOOP            | 
POST_LOOP            | 
POST_GAME            | 

# struct `swGame` <a id="structsw_game"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public swWorld `[`world`](#structsw_game_1af758b85c46fab83ba524687ba3e9900c) | 
`public std::vector< swModel > `[`models`](#structsw_game_1a72f888ef25098f1a0f88cf375073ff50) | 
`public std::vector< swMaterial > `[`materials`](#structsw_game_1afadb80ddbfc9c035376bb80ab15ba338) | 
`public std::vector< swLevel > `[`levels`](#structsw_game_1a00d1a4ca3e4a0e7e6bea28c439cb7343) | 
`public std::vector< swScript > `[`scripts`](#structsw_game_1a07801f647bb4226c67c90ae63870b107) | 
`public std::vector< swEtc > `[`etc`](#structsw_game_1a3f435b576fd71fd77996d87408eb1fc2) | 
`public std::vector< swTexture > `[`textures`](#structsw_game_1a98debae47ea9005492053bee56acace3) | 
`public std::vector< swAudio > `[`audio`](#structsw_game_1af35831ba611871c850b6df9230c56618) | 

## Members

#### `public swWorld `[`world`](#structsw_game_1af758b85c46fab83ba524687ba3e9900c) <a id="structsw_game_1af758b85c46fab83ba524687ba3e9900c"></a>

#### `public std::vector< swModel > `[`models`](#structsw_game_1a72f888ef25098f1a0f88cf375073ff50) <a id="structsw_game_1a72f888ef25098f1a0f88cf375073ff50"></a>

#### `public std::vector< swMaterial > `[`materials`](#structsw_game_1afadb80ddbfc9c035376bb80ab15ba338) <a id="structsw_game_1afadb80ddbfc9c035376bb80ab15ba338"></a>

#### `public std::vector< swLevel > `[`levels`](#structsw_game_1a00d1a4ca3e4a0e7e6bea28c439cb7343) <a id="structsw_game_1a00d1a4ca3e4a0e7e6bea28c439cb7343"></a>

#### `public std::vector< swScript > `[`scripts`](#structsw_game_1a07801f647bb4226c67c90ae63870b107) <a id="structsw_game_1a07801f647bb4226c67c90ae63870b107"></a>

#### `public std::vector< swEtc > `[`etc`](#structsw_game_1a3f435b576fd71fd77996d87408eb1fc2) <a id="structsw_game_1a3f435b576fd71fd77996d87408eb1fc2"></a>

#### `public std::vector< swTexture > `[`textures`](#structsw_game_1a98debae47ea9005492053bee56acace3) <a id="structsw_game_1a98debae47ea9005492053bee56acace3"></a>

#### `public std::vector< swAudio > `[`audio`](#structsw_game_1af35831ba611871c850b6df9230c56618) <a id="structsw_game_1af35831ba611871c850b6df9230c56618"></a>

# struct `sWindow` <a id="structs_window"></a>

Window structure.

This structure represents a window created by the window module. It contains various properties and methods for interacting with the window.

Certain members of this structure are meant to be not accessed directly, but cannot be made private due to the internal implementation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public double `[`dt`](#structs_window_1a7f9230324980123678753482ec6535b6) | Delta time since the last frame.
`public int `[`width`](#structs_window_1adc0627042bc57195d20a7252b32025d6) | The width of the window.
`public int `[`height`](#structs_window_1a453cdecf356a4c1d7ef3eaa56e2ce875) | The height of the window.
`public sWindowFlags `[`flags`](#structs_window_1aca9ac2d2e4917b2a40f531599812baa1) | The window flags.
`public void * `[`internal`](#structs_window_1aadb4ff0f06902a4c4a5a3f1ef62dec38) | 
`public `[`WindowModule`](#struct_window_module)` * `[`creator`](#structs_window_1a7e331b3120a3a648c471e61f4989abd2) | 
`public double `[`lastTime`](#structs_window_1a6bb4638d197ffc97a420a381606067ca) | 
`public std::chrono::high_resolution_clock::time_point `[`startTime`](#structs_window_1a037fa40356fa617401e4a22d0302c307) | 
`public bool `[`did_resize`](#structs_window_1ae5a2257e91903267ae15698ae51dd672) | 

## Members

#### `public double `[`dt`](#structs_window_1a7f9230324980123678753482ec6535b6) <a id="structs_window_1a7f9230324980123678753482ec6535b6"></a>

Delta time since the last frame.

This is the time in seconds since the last frame was rendered.

#### `public int `[`width`](#structs_window_1adc0627042bc57195d20a7252b32025d6) <a id="structs_window_1adc0627042bc57195d20a7252b32025d6"></a>

The width of the window.

This is the width of the window in pixels.

#### `public int `[`height`](#structs_window_1a453cdecf356a4c1d7ef3eaa56e2ce875) <a id="structs_window_1a453cdecf356a4c1d7ef3eaa56e2ce875"></a>

The height of the window.

This is the height of the window in pixels.

#### `public sWindowFlags `[`flags`](#structs_window_1aca9ac2d2e4917b2a40f531599812baa1) <a id="structs_window_1aca9ac2d2e4917b2a40f531599812baa1"></a>

The window flags.

This is a structure containing various properties of the window. 
**See also**: sWindowFlags_t

#### `public void * `[`internal`](#structs_window_1aadb4ff0f06902a4c4a5a3f1ef62dec38) <a id="structs_window_1aadb4ff0f06902a4c4a5a3f1ef62dec38"></a>

#### `public `[`WindowModule`](#struct_window_module)` * `[`creator`](#structs_window_1a7e331b3120a3a648c471e61f4989abd2) <a id="structs_window_1a7e331b3120a3a648c471e61f4989abd2"></a>

#### `public double `[`lastTime`](#structs_window_1a6bb4638d197ffc97a420a381606067ca) <a id="structs_window_1a6bb4638d197ffc97a420a381606067ca"></a>

#### `public std::chrono::high_resolution_clock::time_point `[`startTime`](#structs_window_1a037fa40356fa617401e4a22d0302c307) <a id="structs_window_1a037fa40356fa617401e4a22d0302c307"></a>

#### `public bool `[`did_resize`](#structs_window_1ae5a2257e91903267ae15698ae51dd672) <a id="structs_window_1ae5a2257e91903267ae15698ae51dd672"></a>

# struct `sWindowFlags` <a id="structs_window_flags"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool `[`vsync`](#structs_window_flags_1ae2b9e4039cfa0be4f2c85c4eb58214c3) | Whether vertical sync is enabled.
`public bool `[`resizable`](#structs_window_flags_1a729ab07e65c58ac8233996eaba286391) | Whether the window is fullscreen.

## Members

#### `public bool `[`vsync`](#structs_window_flags_1ae2b9e4039cfa0be4f2c85c4eb58214c3) <a id="structs_window_flags_1ae2b9e4039cfa0be4f2c85c4eb58214c3"></a>

Whether vertical sync is enabled.

If true, the window will synchronize its frame rate with the monitor's refresh rate. This may help reduce screen tearing and improve performance. 

This may not be supported on all platforms or windowing libraries.

#### `public bool `[`resizable`](#structs_window_flags_1a729ab07e65c58ac8233996eaba286391) <a id="structs_window_flags_1a729ab07e65c58ac8233996eaba286391"></a>

Whether the window is fullscreen.

If true, the window will be displayed in fullscreen mode. This may not be supported on all platforms or windowing libraries.

# struct `swLevel` <a id="structsw_level"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< swLevelObject > `[`objects`](#structsw_level_1ae4f18a4e9c8cf77d45d87858c8e19b1d) | 

## Members

#### `public std::vector< swLevelObject > `[`objects`](#structsw_level_1ae4f18a4e9c8cf77d45d87858c8e19b1d) <a id="structsw_level_1ae4f18a4e9c8cf77d45d87858c8e19b1d"></a>

# struct `swLevelObject` <a id="structsw_level_object"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public sModelTransform `[`transform`](#structsw_level_object_1a9ff2fc7eba5f2d724880cddbf8037409) | 
`public void * `[`ecsObject`](#structsw_level_object_1a3ba1acbfae093deab2a1cbddae682926) | 

## Members

#### `public sModelTransform `[`transform`](#structsw_level_object_1a9ff2fc7eba5f2d724880cddbf8037409) <a id="structsw_level_object_1a9ff2fc7eba5f2d724880cddbf8037409"></a>

#### `public void * `[`ecsObject`](#structsw_level_object_1a3ba1acbfae093deab2a1cbddae682926) <a id="structsw_level_object_1a3ba1acbfae093deab2a1cbddae682926"></a>

# struct `swMaterial` <a id="structsw_material"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string `[`shader`](#structsw_material_1a451d8feb730d970fbe3c8695ae58738b) | 
`public std::vector< std::string > `[`samplers`](#structsw_material_1a49c4baa3450fd5a68da40cb944f7906b) | 
`public std::vector< std::string > `[`vertexUniforms`](#structsw_material_1abe26728a1d90033ffb4e271861975a70) | 
`public std::vector< std::string > `[`fragmentUniforms`](#structsw_material_1a3976c6e04509c5c3a40f43ff079372c1) | 

## Members

#### `public std::string `[`shader`](#structsw_material_1a451d8feb730d970fbe3c8695ae58738b) <a id="structsw_material_1a451d8feb730d970fbe3c8695ae58738b"></a>

#### `public std::vector< std::string > `[`samplers`](#structsw_material_1a49c4baa3450fd5a68da40cb944f7906b) <a id="structsw_material_1a49c4baa3450fd5a68da40cb944f7906b"></a>

#### `public std::vector< std::string > `[`vertexUniforms`](#structsw_material_1abe26728a1d90033ffb4e271861975a70) <a id="structsw_material_1abe26728a1d90033ffb4e271861975a70"></a>

#### `public std::vector< std::string > `[`fragmentUniforms`](#structsw_material_1a3976c6e04509c5c3a40f43ff079372c1) <a id="structsw_material_1a3976c6e04509c5c3a40f43ff079372c1"></a>

# struct `swModel` <a id="structsw_model"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< float > `[`vertices`](#structsw_model_1ae1ad99a88224ca4dc27811d2f4cc53b2) | 
`public std::vector< unsigned int > `[`indices`](#structsw_model_1afb4fdd19bdc0697c4fe5dea20a8559d5) | 

## Members

#### `public std::vector< float > `[`vertices`](#structsw_model_1ae1ad99a88224ca4dc27811d2f4cc53b2) <a id="structsw_model_1ae1ad99a88224ca4dc27811d2f4cc53b2"></a>

#### `public std::vector< unsigned int > `[`indices`](#structsw_model_1afb4fdd19bdc0697c4fe5dea20a8559d5) <a id="structsw_model_1afb4fdd19bdc0697c4fe5dea20a8559d5"></a>

# struct `swScript` <a id="structsw_script"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string `[`ext`](#structsw_script_1a60672c7b31ae9e98f2a8ade853a69b99) | 
`public std::string `[`mod`](#structsw_script_1ad05e2748326d28d9001961df7cf6a901) | 

## Members

#### `public std::string `[`ext`](#structsw_script_1a60672c7b31ae9e98f2a8ade853a69b99) <a id="structsw_script_1a60672c7b31ae9e98f2a8ade853a69b99"></a>

#### `public std::string `[`mod`](#structsw_script_1ad05e2748326d28d9001961df7cf6a901) <a id="structsw_script_1ad05e2748326d28d9001961df7cf6a901"></a>

# struct `swTexture` <a id="structsw_texture"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string `[`path`](#structsw_texture_1a08283f0ef25b5c69cd8d249e65f9d955) | 
`public std::vector< uint8_t > `[`data`](#structsw_texture_1ae9ea9a9564f1bcc834407f3f3e4b7eae) | 

## Members

#### `public std::string `[`path`](#structsw_texture_1a08283f0ef25b5c69cd8d249e65f9d955) <a id="structsw_texture_1a08283f0ef25b5c69cd8d249e65f9d955"></a>

#### `public std::vector< uint8_t > `[`data`](#structsw_texture_1ae9ea9a9564f1bcc834407f3f3e4b7eae) <a id="structsw_texture_1ae9ea9a9564f1bcc834407f3f3e4b7eae"></a>

# struct `swWorld` <a id="structsw_world"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string `[`name`](#structsw_world_1aeddc4cf2d208f44fcea210c5cbfd9824) | 
`public std::string `[`author`](#structsw_world_1acb0cbcbca9d9cff9a84af0ca7f584de6) | 
`public std::string `[`description`](#structsw_world_1a7d9bcaa6060b663f017fcafbb1222455) | 

## Members

#### `public std::string `[`name`](#structsw_world_1aeddc4cf2d208f44fcea210c5cbfd9824) <a id="structsw_world_1aeddc4cf2d208f44fcea210c5cbfd9824"></a>

#### `public std::string `[`author`](#structsw_world_1acb0cbcbca9d9cff9a84af0ca7f584de6) <a id="structsw_world_1acb0cbcbca9d9cff9a84af0ca7f584de6"></a>

#### `public std::string `[`description`](#structsw_world_1a7d9bcaa6060b663f017fcafbb1222455) <a id="structsw_world_1a7d9bcaa6060b663f017fcafbb1222455"></a>

# struct `TextAssetBuffer` <a id="struct_text_asset_buffer"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public const char * `[`data`](#struct_text_asset_buffer_1a24a59c388d04469d287b5649a6dc1d4b) | 
`public size_t `[`len`](#struct_text_asset_buffer_1a3387757c5f439d6b38f00452c559f3b0) | 

## Members

#### `public const char * `[`data`](#struct_text_asset_buffer_1a24a59c388d04469d287b5649a6dc1d4b) <a id="struct_text_asset_buffer_1a24a59c388d04469d287b5649a6dc1d4b"></a>

#### `public size_t `[`len`](#struct_text_asset_buffer_1a3387757c5f439d6b38f00452c559f3b0) <a id="struct_text_asset_buffer_1a3387757c5f439d6b38f00452c559f3b0"></a>

# struct `TextModule` <a id="struct_text_module"></a>

```
struct TextModule
  : public Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public text::Init `[`init`](#struct_text_module_1a8a2f31481840637eca26e6863da0aca6) | 
`public text::LoadFont `[`internal_loadFont`](#struct_text_module_1a0619de2ed02a2c9d94d64b8c098ccaec) | 
`public text::LoadFontAsset `[`internal_loadFontAsset`](#struct_text_module_1ac833eee3f8e53ae972cde49685cdd198) | 
`public text::CreateText `[`createText`](#struct_text_module_1a888e5ec2da9e24ab61174273a5c81932) | 
`public text::DrawText `[`drawText`](#struct_text_module_1a9cb778a7ffd4e85925c3200acba59e72) | 
`public text::FreeText `[`freeText`](#struct_text_module_1ab0f3371867b96669b2dfa1071cfbafcf) | 
`public text::FreeFont `[`freeFont`](#struct_text_module_1af9926bec2a36edc173af9aea2de2fef7) | 
`public text::SetTextColor `[`setTextColor`](#struct_text_module_1a8ecfc24c9711ab7e648d21ea028b3804) | 
`public text::SetTextModel `[`setTextModel`](#struct_text_module_1a9d1e4f4c14b9594979f32496627f7568) | 
`public text::SetTextView `[`setTextView`](#struct_text_module_1a397aa2f9a1db1d64ac20173cf3eaa310) | 
`public text::SetTextProj `[`setTextProj`](#struct_text_module_1aa2cfa0e102eeaf49c1dee594654491e4) | 
`public text::MeasureText `[`measureText`](#struct_text_module_1aa238dd4cf00b46fe5c18f67e2b377940) | 
`public text::SetTextZ `[`setTextZ`](#struct_text_module_1a5ade4bfd014881acd5ba714329d1a472) | 
`public inline sFont `[`loadFont`](#struct_text_module_1a3c17f97f54ef29abfa84c946fc5f9905)`(const char * path,int size,const char * vertpath,const char * fragpath)` | 
`public inline sFont `[`loadFontAsset`](#struct_text_module_1a5801ab765f08be602a4af480a83061d0)`(const char * path,int size,const char * vertpath,const char * fragpath)` | 
`public inline  `[`TextModule`](#struct_text_module_1a692bd0c26d18e518f91aee8e32e3a337)`(const char * path)` | 

## Members

#### `public text::Init `[`init`](#struct_text_module_1a8a2f31481840637eca26e6863da0aca6) <a id="struct_text_module_1a8a2f31481840637eca26e6863da0aca6"></a>

#### `public text::LoadFont `[`internal_loadFont`](#struct_text_module_1a0619de2ed02a2c9d94d64b8c098ccaec) <a id="struct_text_module_1a0619de2ed02a2c9d94d64b8c098ccaec"></a>

#### `public text::LoadFontAsset `[`internal_loadFontAsset`](#struct_text_module_1ac833eee3f8e53ae972cde49685cdd198) <a id="struct_text_module_1ac833eee3f8e53ae972cde49685cdd198"></a>

#### `public text::CreateText `[`createText`](#struct_text_module_1a888e5ec2da9e24ab61174273a5c81932) <a id="struct_text_module_1a888e5ec2da9e24ab61174273a5c81932"></a>

#### `public text::DrawText `[`drawText`](#struct_text_module_1a9cb778a7ffd4e85925c3200acba59e72) <a id="struct_text_module_1a9cb778a7ffd4e85925c3200acba59e72"></a>

#### `public text::FreeText `[`freeText`](#struct_text_module_1ab0f3371867b96669b2dfa1071cfbafcf) <a id="struct_text_module_1ab0f3371867b96669b2dfa1071cfbafcf"></a>

#### `public text::FreeFont `[`freeFont`](#struct_text_module_1af9926bec2a36edc173af9aea2de2fef7) <a id="struct_text_module_1af9926bec2a36edc173af9aea2de2fef7"></a>

#### `public text::SetTextColor `[`setTextColor`](#struct_text_module_1a8ecfc24c9711ab7e648d21ea028b3804) <a id="struct_text_module_1a8ecfc24c9711ab7e648d21ea028b3804"></a>

#### `public text::SetTextModel `[`setTextModel`](#struct_text_module_1a9d1e4f4c14b9594979f32496627f7568) <a id="struct_text_module_1a9d1e4f4c14b9594979f32496627f7568"></a>

#### `public text::SetTextView `[`setTextView`](#struct_text_module_1a397aa2f9a1db1d64ac20173cf3eaa310) <a id="struct_text_module_1a397aa2f9a1db1d64ac20173cf3eaa310"></a>

#### `public text::SetTextProj `[`setTextProj`](#struct_text_module_1aa2cfa0e102eeaf49c1dee594654491e4) <a id="struct_text_module_1aa2cfa0e102eeaf49c1dee594654491e4"></a>

#### `public text::MeasureText `[`measureText`](#struct_text_module_1aa238dd4cf00b46fe5c18f67e2b377940) <a id="struct_text_module_1aa238dd4cf00b46fe5c18f67e2b377940"></a>

#### `public text::SetTextZ `[`setTextZ`](#struct_text_module_1a5ade4bfd014881acd5ba714329d1a472) <a id="struct_text_module_1a5ade4bfd014881acd5ba714329d1a472"></a>

#### `public inline sFont `[`loadFont`](#struct_text_module_1a3c17f97f54ef29abfa84c946fc5f9905)`(const char * path,int size,const char * vertpath,const char * fragpath)` <a id="struct_text_module_1a3c17f97f54ef29abfa84c946fc5f9905"></a>

#### `public inline sFont `[`loadFontAsset`](#struct_text_module_1a5801ab765f08be602a4af480a83061d0)`(const char * path,int size,const char * vertpath,const char * fragpath)` <a id="struct_text_module_1a5801ab765f08be602a4af480a83061d0"></a>

#### `public inline  `[`TextModule`](#struct_text_module_1a692bd0c26d18e518f91aee8e32e3a337)`(const char * path)` <a id="struct_text_module_1a692bd0c26d18e518f91aee8e32e3a337"></a>

# struct `TextureModule` <a id="struct_texture_module"></a>

```
struct TextureModule
  : public Module
```  

Texture loading module class.

This class represents the texture loading module and provides methods for loading and freeing textures. It is responsible for loading the texture loading library and providing access to its functions.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline `[`sTextureDefinition`](#structs_texture_definition)` `[`loadTexture`](#struct_texture_module_1ac9a761eaf80efd9ed35ec08dda8bb3c3)`(const char * filepath)` | Loads a texture from a file path.
`public inline void `[`freeTexture`](#struct_texture_module_1a2e017f0716ef1c6f5587457df89e186b)`(`[`sTextureDefinition`](#structs_texture_definition)` texture)` | Frees a texture definition.
`public inline `[`sTextureDefinition`](#structs_texture_definition)` `[`loadTextureFromBuffer`](#struct_texture_module_1acb8b5c1eecedd84e01f8032492fad92e)`(const uint8_t * buffer,size_t size)` | Loads a texture from a buffer.
`public inline  `[`TextureModule`](#struct_texture_module_1aa3cc7def4aca8b67aac66385234682d0)`(const char * path)` | Constructor for the [TextureModule](#struct_texture_module) class.

## Members

#### `public inline `[`sTextureDefinition`](#structs_texture_definition)` `[`loadTexture`](#struct_texture_module_1ac9a761eaf80efd9ed35ec08dda8bb3c3)`(const char * filepath)` <a id="struct_texture_module_1ac9a761eaf80efd9ed35ec08dda8bb3c3"></a>

Loads a texture from a file path.

This function loads a texture from the specified file path and returns a texture definition.

#### Parameters
* `filepath` The path to the texture to load 

#### Returns
A texture definition containing the loaded texture data.

#### `public inline void `[`freeTexture`](#struct_texture_module_1a2e017f0716ef1c6f5587457df89e186b)`(`[`sTextureDefinition`](#structs_texture_definition)` texture)` <a id="struct_texture_module_1a2e017f0716ef1c6f5587457df89e186b"></a>

Frees a texture definition.

This function frees the specified texture definition. This should be called when the texture is no longer needed to free the resources used by the texture.

#### Parameters
* `texture` The texture definition to free. 

This function should be called when the texture is no longer needed to free up resources.

#### `public inline `[`sTextureDefinition`](#structs_texture_definition)` `[`loadTextureFromBuffer`](#struct_texture_module_1acb8b5c1eecedd84e01f8032492fad92e)`(const uint8_t * buffer,size_t size)` <a id="struct_texture_module_1acb8b5c1eecedd84e01f8032492fad92e"></a>

Loads a texture from a buffer.

This function loads a texture from the specified buffer and size, and returns a texture definition.

#### Parameters
* `buffer` The byte buffer containing the texture data. 

* `size` The size of the byte buffer. 

#### Returns
A texture definition containing the loaded texture data.

#### `public inline  `[`TextureModule`](#struct_texture_module_1aa3cc7def4aca8b67aac66385234682d0)`(const char * path)` <a id="struct_texture_module_1aa3cc7def4aca8b67aac66385234682d0"></a>

Constructor for the [TextureModule](#struct_texture_module) class.

This constructor loads the texture loading library and initializes the function pointers.

#### Parameters
* `path` The name/id of the implementation to load (ex: "stb").

# struct `vec2.__unnamed11__` <a id="structvec2_8____unnamed11____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`u`](#structvec2_8____unnamed11_____1a7b774effe4a349c6dd82ad4f4f21d34c) | 
`public float `[`v`](#structvec2_8____unnamed11_____1a9e3669d19b675bd57058fd4664205d2a) | 

## Members

#### `public float `[`u`](#structvec2_8____unnamed11_____1a7b774effe4a349c6dd82ad4f4f21d34c) <a id="structvec2_8____unnamed11_____1a7b774effe4a349c6dd82ad4f4f21d34c"></a>

#### `public float `[`v`](#structvec2_8____unnamed11_____1a9e3669d19b675bd57058fd4664205d2a) <a id="structvec2_8____unnamed11_____1a9e3669d19b675bd57058fd4664205d2a"></a>

# struct `vec2.__unnamed9__` <a id="structvec2_8____unnamed9____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`x`](#structvec2_8____unnamed9_____1a9dd4e461268c8034f5c8564e155c67a6) | 
`public float `[`y`](#structvec2_8____unnamed9_____1a415290769594460e2e485922904f345d) | 

## Members

#### `public float `[`x`](#structvec2_8____unnamed9_____1a9dd4e461268c8034f5c8564e155c67a6) <a id="structvec2_8____unnamed9_____1a9dd4e461268c8034f5c8564e155c67a6"></a>

#### `public float `[`y`](#structvec2_8____unnamed9_____1a415290769594460e2e485922904f345d) <a id="structvec2_8____unnamed9_____1a415290769594460e2e485922904f345d"></a>

# struct `vec3.__unnamed1__` <a id="structvec3_8____unnamed1____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`x`](#structvec3_8____unnamed1_____1a9dd4e461268c8034f5c8564e155c67a6) | 
`public float `[`y`](#structvec3_8____unnamed1_____1a415290769594460e2e485922904f345d) | 
`public float `[`z`](#structvec3_8____unnamed1_____1afbade9e36a3f36d3d676c1b808451dd7) | 

## Members

#### `public float `[`x`](#structvec3_8____unnamed1_____1a9dd4e461268c8034f5c8564e155c67a6) <a id="structvec3_8____unnamed1_____1a9dd4e461268c8034f5c8564e155c67a6"></a>

#### `public float `[`y`](#structvec3_8____unnamed1_____1a415290769594460e2e485922904f345d) <a id="structvec3_8____unnamed1_____1a415290769594460e2e485922904f345d"></a>

#### `public float `[`z`](#structvec3_8____unnamed1_____1afbade9e36a3f36d3d676c1b808451dd7) <a id="structvec3_8____unnamed1_____1afbade9e36a3f36d3d676c1b808451dd7"></a>

# struct `vec3.__unnamed3__` <a id="structvec3_8____unnamed3____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`r`](#structvec3_8____unnamed3_____1a4b43b0aee35624cd95b910189b3dc231) | 
`public float `[`g`](#structvec3_8____unnamed3_____1ab2f5ff47436671b6e533d8dc3614845d) | 
`public float `[`b`](#structvec3_8____unnamed3_____1a92eb5ffee6ae2fec3ad71c777531578f) | 

## Members

#### `public float `[`r`](#structvec3_8____unnamed3_____1a4b43b0aee35624cd95b910189b3dc231) <a id="structvec3_8____unnamed3_____1a4b43b0aee35624cd95b910189b3dc231"></a>

#### `public float `[`g`](#structvec3_8____unnamed3_____1ab2f5ff47436671b6e533d8dc3614845d) <a id="structvec3_8____unnamed3_____1ab2f5ff47436671b6e533d8dc3614845d"></a>

#### `public float `[`b`](#structvec3_8____unnamed3_____1a92eb5ffee6ae2fec3ad71c777531578f) <a id="structvec3_8____unnamed3_____1a92eb5ffee6ae2fec3ad71c777531578f"></a>

# struct `vec4.__unnamed5__` <a id="structvec4_8____unnamed5____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`x`](#structvec4_8____unnamed5_____1a9dd4e461268c8034f5c8564e155c67a6) | 
`public float `[`y`](#structvec4_8____unnamed5_____1a415290769594460e2e485922904f345d) | 
`public float `[`z`](#structvec4_8____unnamed5_____1afbade9e36a3f36d3d676c1b808451dd7) | 
`public float `[`w`](#structvec4_8____unnamed5_____1af1290186a5d0b1ceab27f4e77c0c5d68) | 

## Members

#### `public float `[`x`](#structvec4_8____unnamed5_____1a9dd4e461268c8034f5c8564e155c67a6) <a id="structvec4_8____unnamed5_____1a9dd4e461268c8034f5c8564e155c67a6"></a>

#### `public float `[`y`](#structvec4_8____unnamed5_____1a415290769594460e2e485922904f345d) <a id="structvec4_8____unnamed5_____1a415290769594460e2e485922904f345d"></a>

#### `public float `[`z`](#structvec4_8____unnamed5_____1afbade9e36a3f36d3d676c1b808451dd7) <a id="structvec4_8____unnamed5_____1afbade9e36a3f36d3d676c1b808451dd7"></a>

#### `public float `[`w`](#structvec4_8____unnamed5_____1af1290186a5d0b1ceab27f4e77c0c5d68) <a id="structvec4_8____unnamed5_____1af1290186a5d0b1ceab27f4e77c0c5d68"></a>

# struct `vec4.__unnamed7__` <a id="structvec4_8____unnamed7____"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`r`](#structvec4_8____unnamed7_____1a4b43b0aee35624cd95b910189b3dc231) | 
`public float `[`g`](#structvec4_8____unnamed7_____1ab2f5ff47436671b6e533d8dc3614845d) | 
`public float `[`b`](#structvec4_8____unnamed7_____1a92eb5ffee6ae2fec3ad71c777531578f) | 
`public float `[`a`](#structvec4_8____unnamed7_____1a0cc175b9c0f1b6a831c399e269772661) | 

## Members

#### `public float `[`r`](#structvec4_8____unnamed7_____1a4b43b0aee35624cd95b910189b3dc231) <a id="structvec4_8____unnamed7_____1a4b43b0aee35624cd95b910189b3dc231"></a>

#### `public float `[`g`](#structvec4_8____unnamed7_____1ab2f5ff47436671b6e533d8dc3614845d) <a id="structvec4_8____unnamed7_____1ab2f5ff47436671b6e533d8dc3614845d"></a>

#### `public float `[`b`](#structvec4_8____unnamed7_____1a92eb5ffee6ae2fec3ad71c777531578f) <a id="structvec4_8____unnamed7_____1a92eb5ffee6ae2fec3ad71c777531578f"></a>

#### `public float `[`a`](#structvec4_8____unnamed7_____1a0cc175b9c0f1b6a831c399e269772661) <a id="structvec4_8____unnamed7_____1a0cc175b9c0f1b6a831c399e269772661"></a>

# struct `Vertex` <a id="struct_vertex"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public float `[`position`](#struct_vertex_1a1a5f22694da8ef6c906956eea8ddc924) | 
`public float `[`normal`](#struct_vertex_1a44dd0d22dccb4684ba4c8cb7400536fc) | 
`public float `[`texcoord`](#struct_vertex_1acfd7bc0aa5fbf2ff334416aeab61ac88) | 

## Members

#### `public float `[`position`](#struct_vertex_1a1a5f22694da8ef6c906956eea8ddc924) <a id="struct_vertex_1a1a5f22694da8ef6c906956eea8ddc924"></a>

#### `public float `[`normal`](#struct_vertex_1a44dd0d22dccb4684ba4c8cb7400536fc) <a id="struct_vertex_1a44dd0d22dccb4684ba4c8cb7400536fc"></a>

#### `public float `[`texcoord`](#struct_vertex_1acfd7bc0aa5fbf2ff334416aeab61ac88) <a id="struct_vertex_1acfd7bc0aa5fbf2ff334416aeab61ac88"></a>

# struct `WindowModule` <a id="struct_window_module"></a>

```
struct WindowModule
  : public Module
```  

Window module class.

This class represents the window module and provides methods for creating and managing windows. It is responsible for loading the windowing library and providing access to its functions.

This class contains function pointers that will be loaded from the module file (a dynamic library). 

These functions will do something different depending on the implementation selected at runtime.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline `[`sWindow`](#structs_window)` * `[`loadWindow`](#struct_window_module_1af37eab1ce1cde80532f09d9b32133184)`(const char * name,int width,int height,sWindowFlags flags)` | Load a window with the specified name, width, height, and flags.
`public inline `[`sWindow`](#structs_window)` * `[`loadWindow`](#struct_window_module_1afbb39fabdd17eabc5fc13c5f35b26a59)`(const char * name,int width,int height,bool vsync,bool resizable)` | Load a window with the specified name, width, height, and default flags.
`public inline void `[`updateWindow`](#struct_window_module_1a332a23ed050d2f60c9aa706580ade3f3)`(`[`sWindow`](#structs_window)` * window)` | Update the window properties and state.
`public inline void `[`destroyWindow`](#struct_window_module_1a7fa71ee2451bb4e8eddc3b419d539e17)`(`[`sWindow`](#structs_window)` * window)` | Destroy the specified window and free its resources.
`public inline void `[`swapBuffers`](#struct_window_module_1aa40ef3b05241f7d5ea3a0cc870ee759e)`(`[`sWindow`](#structs_window)` window)` | Swap the front and back buffers of the specified window.
`public inline bool `[`shouldClose`](#struct_window_module_1a2b7146e8692258106652425239321ad7)`(`[`sWindow`](#structs_window)` window)` | Check if the specified window should close.
`public inline void `[`setShouldClose`](#struct_window_module_1a08ad70a8d266990be05e96d5ebe0598c)`(`[`sWindow`](#structs_window)` window,bool value)` | Set the specified window to close or not.
`public inline void * `[`getHandle`](#struct_window_module_1af74f66ec626c21671d099438d3b20b8e)`(`[`sWindow`](#structs_window)` window)` | Get the handle of the specified window.
`public inline bool `[`isKeyPressed`](#struct_window_module_1a4a91e784850f9b2ae05f6a9c501046f3)`(`[`sWindow`](#structs_window)` window,Key key)` | Check if a specific key is pressed in the specified window.
`public inline bool `[`isMouseButtonPressed`](#struct_window_module_1acac8648642874b659af30157ae9827f8)`(`[`sWindow`](#structs_window)` window,int button)` | Check if a specific mouse button is pressed in the specified window.
`public inline void `[`getMousePosition`](#struct_window_module_1a8dab3e5fdea2a88d1c0ec74605860a4e)`(`[`sWindow`](#structs_window)` window,float * x,float * y)` | Get the current mouse position in the specified window.
`public inline void `[`setMousePosition`](#struct_window_module_1aa6d4f9973a482898067469b021c0e754)`(`[`sWindow`](#structs_window)` window,float x,float y)` | Set the mouse position in the specified window.
`public inline void `[`setCursorMode`](#struct_window_module_1a80067a92ac8bbe1a14aba39a1e545ccf)`(`[`sWindow`](#structs_window)` window,`[`CursorMode`](#win_2module_8h_1a3bbf59221044115f117dc97bb8930648)` mode)` | Set the cursor mode for the specified window.
`public inline void `[`setWindowTitle`](#struct_window_module_1ac1110dafa5d9c59fb5b9cd271d87063f)`(`[`sWindow`](#structs_window)` window,const char * title)` | Set the title of the specified window.
`public inline double `[`getTime`](#struct_window_module_1a9efdb8131b23be2226b841d1969265a2)`(`[`sWindow`](#structs_window)` window)` | Get the time since the window was created.
`public inline  explicit `[`WindowModule`](#struct_window_module_1a2427e19698191d79b01bde5e380533ff)`(const char * dynlib)` | Constructor for the [WindowModule](#struct_window_module) class.

## Members

#### `public inline `[`sWindow`](#structs_window)` * `[`loadWindow`](#struct_window_module_1af37eab1ce1cde80532f09d9b32133184)`(const char * name,int width,int height,sWindowFlags flags)` <a id="struct_window_module_1af37eab1ce1cde80532f09d9b32133184"></a>

Load a window with the specified name, width, height, and flags.

This function creates a new window with the specified properties and returns a pointer to it.

#### Parameters
* `name` The name of the window. 

* `width` The width of the window in pixels. 

* `height` The height of the window in pixels. 

* `flags` The window flags (e.g., vsync, resizable). 

#### Returns
A pointer to the created window.

#### `public inline `[`sWindow`](#structs_window)` * `[`loadWindow`](#struct_window_module_1afbb39fabdd17eabc5fc13c5f35b26a59)`(const char * name,int width,int height,bool vsync,bool resizable)` <a id="struct_window_module_1afbb39fabdd17eabc5fc13c5f35b26a59"></a>

Load a window with the specified name, width, height, and default flags.

This function creates a new window with the specified properties and default flags (vsync=true, resizable=false).

#### Parameters
* `name` The name of the window. 

* `width` The width of the window in pixels. 

* `height` The height of the window in pixels. 

* `vsync` Whether vertical sync is enabled (default: true). 

* `resizable` Whether the window is resizable (default: false). 

#### Returns
A pointer to the created window.

#### `public inline void `[`updateWindow`](#struct_window_module_1a332a23ed050d2f60c9aa706580ade3f3)`(`[`sWindow`](#structs_window)` * window)` <a id="struct_window_module_1a332a23ed050d2f60c9aa706580ade3f3"></a>

Update the window properties and state.

This function updates the window properties and state, including the delta time since the last frame.

#### Parameters
* `window` A pointer to the window to update. 

This function should be called once per frame to ensure the window is updated correctly.

#### `public inline void `[`destroyWindow`](#struct_window_module_1a7fa71ee2451bb4e8eddc3b419d539e17)`(`[`sWindow`](#structs_window)` * window)` <a id="struct_window_module_1a7fa71ee2451bb4e8eddc3b419d539e17"></a>

Destroy the specified window and free its resources.

This function destroys the specified window and frees any resources associated with it.

#### Parameters
* `window` A pointer to the window to destroy. 

This function should be called when the window is no longer needed to free up resources.

#### `public inline void `[`swapBuffers`](#struct_window_module_1aa40ef3b05241f7d5ea3a0cc870ee759e)`(`[`sWindow`](#structs_window)` window)` <a id="struct_window_module_1aa40ef3b05241f7d5ea3a0cc870ee759e"></a>

Swap the front and back buffers of the specified window.

This function swaps the front and back buffers of the specified window, displaying the rendered content.

#### Parameters
* `window` A pointer to the window to swap buffers for. 

This function should be called after rendering to the window to display the content.

#### `public inline bool `[`shouldClose`](#struct_window_module_1a2b7146e8692258106652425239321ad7)`(`[`sWindow`](#structs_window)` window)` <a id="struct_window_module_1a2b7146e8692258106652425239321ad7"></a>

Check if the specified window should close.

This function checks if the specified window should close based on its internal state.

#### Parameters
* `window` A pointer to the window to check. 

#### Returns
true if the window should close, false otherwise. 

This function is typically used to determine if the window should be closed based on user input or other conditions.

#### `public inline void `[`setShouldClose`](#struct_window_module_1a08ad70a8d266990be05e96d5ebe0598c)`(`[`sWindow`](#structs_window)` window,bool value)` <a id="struct_window_module_1a08ad70a8d266990be05e96d5ebe0598c"></a>

Set the specified window to close or not.

This function sets the specified window to close or not based on the provided value.

#### Parameters
* `window` A pointer to the window to set. 

* `value` true to set the window to close, false to keep it open. 

This function is typically used to programmatically close the window based on user input or other conditions. 

This function may not be supported on all platforms or windowing libraries.

#### `public inline void * `[`getHandle`](#struct_window_module_1af74f66ec626c21671d099438d3b20b8e)`(`[`sWindow`](#structs_window)` window)` <a id="struct_window_module_1af74f66ec626c21671d099438d3b20b8e"></a>

Get the handle of the specified window.

This function retrieves the handle of the specified window, which can be used for platform-specific operations.

#### Parameters
* `window` A pointer to the window to get the handle for. 

#### Returns
A pointer to the handle of the window. 

Unlike the other functions, this one MUST be supported by all windowing libraries for the graphics module to work.

#### `public inline bool `[`isKeyPressed`](#struct_window_module_1a4a91e784850f9b2ae05f6a9c501046f3)`(`[`sWindow`](#structs_window)` window,Key key)` <a id="struct_window_module_1a4a91e784850f9b2ae05f6a9c501046f3"></a>

Check if a specific key is pressed in the specified window.

This function checks if a specific key is currently pressed in the specified window.

#### Parameters
* `window` A pointer to the window to check. 

* `key` The key to check for (e.g., Key::A, Key::Escape). 

#### Returns
true if the key is pressed, false otherwise. 

This should not be relied on for gameplay, as there will be an input module for that 

> Todo: Update this once the input module is implemented.

#### `public inline bool `[`isMouseButtonPressed`](#struct_window_module_1acac8648642874b659af30157ae9827f8)`(`[`sWindow`](#structs_window)` window,int button)` <a id="struct_window_module_1acac8648642874b659af30157ae9827f8"></a>

Check if a specific mouse button is pressed in the specified window.

This function checks if a specific mouse button is currently pressed in the specified window.

#### Parameters
* `window` A pointer to the window to check. 

* `button` The mouse button to check for (e.g., 0 for left button, 1 for right button). 

#### Returns
true if the mouse button is pressed, false otherwise. 

This should not be relied on for gameplay, as there will be an input module for that 

> Todo: Update this once the input module is implemented.

#### `public inline void `[`getMousePosition`](#struct_window_module_1a8dab3e5fdea2a88d1c0ec74605860a4e)`(`[`sWindow`](#structs_window)` window,float * x,float * y)` <a id="struct_window_module_1a8dab3e5fdea2a88d1c0ec74605860a4e"></a>

Get the current mouse position in the specified window.

This function retrieves the current mouse position in the specified window.

#### Parameters
* `window` A pointer to the window to get the mouse position for. 

* `x` A pointer to store the x-coordinate of the mouse position. 

* `y` A pointer to store the y-coordinate of the mouse position. 

The coordinates might be relative to the window or the screen, depending on the windowing library, this should only be used for calculating deltas. 

This should not be relied on for gameplay, as there will be an input module for that 

> Todo: Update this once the input module is implemented.

#### `public inline void `[`setMousePosition`](#struct_window_module_1aa6d4f9973a482898067469b021c0e754)`(`[`sWindow`](#structs_window)` window,float x,float y)` <a id="struct_window_module_1aa6d4f9973a482898067469b021c0e754"></a>

Set the mouse position in the specified window.

This function sets the mouse position in the specified window.

#### Parameters
* `window` A pointer to the window to set the mouse position for. 

* `x` The x-coordinate of the mouse position. 

* `y` The y-coordinate of the mouse position. 

This might not be supported on all platforms or windowing libraries. 

This should not be relied on for gameplay, as there will be an input module for that 

> Todo: Update this once the input module is implemented.

#### `public inline void `[`setCursorMode`](#struct_window_module_1a80067a92ac8bbe1a14aba39a1e545ccf)`(`[`sWindow`](#structs_window)` window,`[`CursorMode`](#win_2module_8h_1a3bbf59221044115f117dc97bb8930648)` mode)` <a id="struct_window_module_1a80067a92ac8bbe1a14aba39a1e545ccf"></a>

Set the cursor mode for the specified window.

This function sets the cursor mode for the specified window.

#### Parameters
* `window` A pointer to the window to set the cursor mode for. 

* `mode` The cursor mode to set (e.g., [CursorMode::Normal](#win_2module_8h_1a3bbf59221044115f117dc97bb8930648a960b44c579bc2f6818d2daaf9e4c16f0), [CursorMode::Hidden](#win_2module_8h_1a3bbf59221044115f117dc97bb8930648a7acdf85c69cc3c5305456a293524386e)). 

This may not be supported on all platforms or windowing libraries. 

This is useful for first-person camera controls or similar applications. 

This should not be relied on for gameplay, as there will be an input module for that 

> Todo: Update this once the input module is implemented. 

**See also**: [CursorMode](#win_2module_8h_1a3bbf59221044115f117dc97bb8930648)

#### `public inline void `[`setWindowTitle`](#struct_window_module_1ac1110dafa5d9c59fb5b9cd271d87063f)`(`[`sWindow`](#structs_window)` window,const char * title)` <a id="struct_window_module_1ac1110dafa5d9c59fb5b9cd271d87063f"></a>

Set the title of the specified window.

This function sets the title of the specified window.

#### Parameters
* `window` A pointer to the window to set the title for. 

* `title` The title to set for the window. 

This may not be supported on all platforms or windowing libraries. 

This is useful for displaying the current state of the application or game in the window title.

#### `public inline double `[`getTime`](#struct_window_module_1a9efdb8131b23be2226b841d1969265a2)`(`[`sWindow`](#structs_window)` window)` <a id="struct_window_module_1a9efdb8131b23be2226b841d1969265a2"></a>

Get the time since the window was created.

This function retrieves the time since the window was created in seconds.

#### Parameters
* `window` A pointer to the window to get the time for. 

#### Returns
The time since the window was created in seconds. 

This can be used for timing purposes, such as calculating the frame rate or elapsed time.

#### `public inline  explicit `[`WindowModule`](#struct_window_module_1a2427e19698191d79b01bde5e380533ff)`(const char * dynlib)` <a id="struct_window_module_1a2427e19698191d79b01bde5e380533ff"></a>

Constructor for the [WindowModule](#struct_window_module) class.

This constructor loads the windowing library and initializes the function pointers.

#### Parameters
* `dynlib` The name of the dynamic library to load. 

This constructor should be called before using any of the window module functions.

# struct `WorldModule` <a id="struct_world_module"></a>

```
struct WorldModule
  : public Module
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public world::LoadGame `[`loadGame`](#struct_world_module_1a7337b8b4968ce0ab8d03b9bc55058cc6) | 
`public world::FreeGame `[`freeGame`](#struct_world_module_1a082c7548f07851beedf7fd9bc90184d9) | 
`public world::SaveGame `[`saveGame`](#struct_world_module_1ab2281577f185e409d1ef8a5562aa3f81) | 
`public inline  `[`WorldModule`](#struct_world_module_1afacaf1af36e69fe165e69759441ea94f)`()` | 

## Members

#### `public world::LoadGame `[`loadGame`](#struct_world_module_1a7337b8b4968ce0ab8d03b9bc55058cc6) <a id="struct_world_module_1a7337b8b4968ce0ab8d03b9bc55058cc6"></a>

#### `public world::FreeGame `[`freeGame`](#struct_world_module_1a082c7548f07851beedf7fd9bc90184d9) <a id="struct_world_module_1a082c7548f07851beedf7fd9bc90184d9"></a>

#### `public world::SaveGame `[`saveGame`](#struct_world_module_1ab2281577f185e409d1ef8a5562aa3f81) <a id="struct_world_module_1ab2281577f185e409d1ef8a5562aa3f81"></a>

#### `public inline  `[`WorldModule`](#struct_world_module_1afacaf1af36e69fe165e69759441ea94f)`()` <a id="struct_world_module_1afacaf1af36e69fe165e69759441ea94f"></a>

Generated by [Moxygen](https://sourcey.com/moxygen)