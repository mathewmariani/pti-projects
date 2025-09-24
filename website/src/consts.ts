export const SITE_TITLE = 'pti-projects';
export const SITE_DESCRIPTION = '';

const folder = import.meta.env.PROD ? 'web-release' : 'web-debug';
export const SYMLINK_SAMPLES_SRC = `build/${folder}/projects/samples`;
export const SYMLINK_SAMPLES_DST = `website/public/projects/samples`;
export const SYMLINK_TESTS_SRC = `build/${folder}/projects/platformer`;
export const SYMLINK_TESTS_DST = `website/public/projects/platformer`;

export const HomeLink = (): string => '/pti-projects/';