from os.path import join
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

env.Replace(
    UPLOADER=join("$PIOPACKAGES_DIR", "tool-openocd", "bin", "openocd"),
    UPLOADERFLAGS=[
        "-d2", "-s",
        join("$PIOPACKAGES_DIR", "tool-openocd",
             "share", "openocd", "scripts"),
        "-f", join("interface", "cmsis-dap.cfg"),
        "-f", join("target", "lpc17xx.cfg"),
        "-c", "\"telnet_port", "disabled;",
        "program", join(env.subst("$BUILD_DIR"), "firmware.bin"),
        "verify", "reset", "0x00000000;", "shutdown\""
    ],
    UPLOADCMD='"$UPLOADER" $UPLOADERFLAGS'
)

upload_dap = env.Alias(["upload_dap"], "$SOURCES", "$UPLOADCMD")
AlwaysBuild(upload_dap)