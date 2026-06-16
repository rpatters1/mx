package corert

import (
	"os"
	"path/filepath"
	"sort"
	"strings"
)

var excludedSegments = map[string]bool{
	"expected":   true,
	"testOutput": true,
	"generalxml": true,
	"smufl":      true,
}

func DiscoverInputFiles(dataRoot string) ([]string, error) {
	var result []string
	err := filepath.Walk(dataRoot, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.Mode().IsRegular() {
			return nil
		}
		rel, err := filepath.Rel(dataRoot, path)
		if err != nil {
			return nil
		}
		if isExcludedPath(rel) {
			return nil
		}
		if !hasXMLExtension(path) {
			return nil
		}
		if isFixupSidecar(path) {
			return nil
		}
		if isAuditArtifact(path) {
			return nil
		}
		if hasInvalidMarker(path) {
			return nil
		}
		result = append(result, path)
		return nil
	})
	if err != nil {
		return nil, err
	}
	sort.Strings(result)
	return result, nil
}

func ToTestName(absPath, dataRoot string) string {
	rel, err := filepath.Rel(dataRoot, absPath)
	if err != nil {
		return absPath
	}
	return filepath.ToSlash(rel)
}

func isExcludedPath(rel string) bool {
	for _, seg := range strings.Split(filepath.ToSlash(rel), "/") {
		if excludedSegments[seg] {
			return true
		}
	}
	return false
}

func hasXMLExtension(path string) bool {
	ext := strings.ToLower(filepath.Ext(path))
	return ext == ".xml" || ext == ".musicxml"
}

func isFixupSidecar(path string) bool {
	return strings.HasSuffix(path, ".fixup.xml")
}

// isAuditArtifact reports whether path is a feature-audit output (a
// *.features.xml sidecar or the corpus.xml aggregate). They share data/'s .xml
// extension but are not MusicXML scores, so the round-trip suite skips them.
func isAuditArtifact(path string) bool {
	return strings.HasSuffix(path, ".features.xml") ||
		filepath.Base(path) == "corpus.xml"
}

func hasInvalidMarker(path string) bool {
	_, err := os.Stat(path + ".invalid")
	return err == nil
}
