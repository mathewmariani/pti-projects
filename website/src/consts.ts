export const SITE_TITLE = 'pti-projects';
export const SITE_DESCRIPTION = '';

const folder = import.meta.env.PROD ? 'web-release' : 'web-debug';
export const SYMLINKS = [
  { source: `build/${folder}/projects/samples`, target: `website/public/projects/samples` },
  { source: `build/${folder}/projects/platformer`, target: `website/public/projects/platformer` },
  { source: `build/${folder}/projects/towerfall-physics`, target: `website/public/projects/towerfall-physics` },
  { source: `build/${folder}/projects/zombies`, target: `website/public/projects/zombies` },
  { source: `build/${folder}/projects/coinrush`, target: `website/public/projects/coinrush` },
  { source: `build/${folder}/projects/rocktoss`, target: `website/public/projects/rocktoss` },
] as const;

export const HomeLink = (): string => '/pti-projects/';