import {
  SYMLINK_SAMPLES_SRC,
  SYMLINK_SAMPLES_DST,
  SYMLINK_TESTS_SRC,
  SYMLINK_TESTS_DST,
} from "../consts";

import type { AstroIntegration } from "astro";
import fs from "fs";
import path from "path";

export const symlinkIntegration = (): AstroIntegration => {
  const symlinks = [
    { source: SYMLINK_SAMPLES_SRC, target: SYMLINK_SAMPLES_DST },
    { source: SYMLINK_TESTS_SRC, target: SYMLINK_TESTS_DST },
  ] as const;

  return {
    name: "astro-lifecycle-logs",
    hooks: {
      "astro:config:setup": async ({ command }) => {
        const suffix = command === "build" ? "Release" : "Debug";
        const symlinkType = process.platform === "win32" ? "junction" : "dir";

        for (const { source, target } of symlinks) {
          const sourcePath = path.resolve(process.cwd(), source, suffix);
          const targetPath = path.resolve(process.cwd(), target);

          fs.mkdirSync(path.dirname(targetPath), { recursive: true });

          if (fs.existsSync(targetPath)) {
            const stat = fs.lstatSync(targetPath);
            if (stat.isSymbolicLink()) fs.unlinkSync(targetPath);
            else {
              console.log(`Destination exists and is not a symlink: ${targetPath}`);
              continue;
            }
          }

          fs.symlinkSync(sourcePath, targetPath, symlinkType);
          console.log(`Symlink created: ${sourcePath} -> ${targetPath}`);
        }
      },
    },
  };
};

export default symlinkIntegration;
