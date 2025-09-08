import argparse
import os
import subprocess


def find_compiler() -> str:
    """!@brief Finds the path of the glslc compiler."""
    sdk_path: str = os.environ.get("VULKAN_SDK")
    if not sdk_path:
        sdk_path = os.environ.get("VK_SDK_PATH")
    if not sdk_path:
        raise RuntimeError("Cannot find the path of the Vulkan SDK")

    compiler_path: str = os.path.join(sdk_path, "bin", "glslc")
    if os.name == "nt":
        compiler_path += ".exe"
    if not os.path.exists(compiler_path):
        raise RuntimeError("Cannot find the glslc compiler")

    return compiler_path


def syntax() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compiles GLSL shaders to SPIR-V")
    parser.add_argument("--shader_name", type=str, nargs="+", help="Shader names")
    parser.add_argument("--shader_path", type=str, nargs="+", help="Shader paths")

    return parser


def main() -> None:
    args: argparse.Namespace = syntax().parse_args()

    # Find the shaders to be compiled
    shaders: list[str] = args.shader_path or []
    if args.shader_name:
        shader_names: tuple[str] = tuple(args.shader_name)

        root_path: str = os.path.dirname(__file__)
        for file in os.listdir(root_path):
            if file.startswith(shader_names):
                shaders.append(os.path.join(root_path, file))

    # Compile shaders
    compiler_path: str = find_compiler()
    for shader in shaders:
        spv_path: str = shader + ".spv"
        subprocess.Popen(f"{compiler_path} {shader} -o {spv_path}")


if __name__ == "__main__":
    main()
