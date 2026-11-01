import fs from 'node:fs';
import path from 'node:path';

const distDir = path.resolve('dist');
const outputFile = path.resolve('../resources.gresource.xml');

const resourcePrefix = '/com/monbebe/audio';

function escapeXml(value) {
  return value
    .replaceAll('&', '&amp;')
    .replaceAll('<', '&lt;')
    .replaceAll('>', '&gt;')
    .replaceAll('"', '&quot;')
    .replaceAll("'", '&apos;');
}

function collectFiles(directory) {
  return fs.readdirSync(directory, { withFileTypes: true })
    .flatMap(entry => {
      const fullPath = path.join(directory, entry.name);

      if (entry.isDirectory()) {
        return collectFiles(fullPath);
      }

      return [fullPath];
    });
}

if (!fs.existsSync(distDir)) {
  throw new Error(`Build directory not found: ${distDir}`);
}

const files = collectFiles(distDir);

const entries = files
  .map(file => {
    const alias = path.relative(distDir, file)
      .split(path.sep)
      .join('/');

    const source = path.relative(
      path.dirname(outputFile),
      file
    ).split(path.sep).join('/');

    // TODO: maybe add compression?
    return `        <file alias="${escapeXml(alias)}">${escapeXml(source)}</file>`;
  })
  .join('\n');

const xml = `<?xml version="1.0" encoding="UTF-8"?>
<gresources>
    <gresource prefix="${resourcePrefix}">
${entries}
    </gresource>
</gresources>
`;

fs.writeFileSync(outputFile, xml);

console.log(`Generated ${outputFile} (${files.length} files)`);
