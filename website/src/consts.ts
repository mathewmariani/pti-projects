export const SITE_TITLE = 'pti-projects';
export const SITE_DESCRIPTION = '';

const folder = import.meta.env.PROD ? 'web-release' : 'web-debug';
export const SYMLINKS = [
  { source: `build/${folder}/projects/samples`, target: `website/public/projects/samples` },
  { source: `build/${folder}/projects/platformer`, target: `website/public/projects/platformer` },
  { source: `build/${folder}/projects/towerfall-physics`, target: `website/public/projects/towerfall-physics` },
  { source: `build/${folder}/projects/zombies`, target: `website/public/projects/zombies` },
] as const;

export const HomeLink = (): string => '/pti-projects/';