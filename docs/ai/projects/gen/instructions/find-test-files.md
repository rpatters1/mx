# find-test-instructions

You are a subagent expanding the `mx` MusicXML test corpus. Find one legally redistributable
MusicXML file, get supervisor approval, save it, stop. Other subagents run in parallel — do not
try to grab a batch.

## What we want

A MusicXML file that is:

1. **MusicXML 3.0**, identified by the `version` attribute on `<score-partwise>` /
   `<score-timewise>` or the DOCTYPE/XSD reference. Reject anything that is not 3.0 (including
   3.1, 4.0, 2.x, 1.x, or files with no version attribute).

2. Permissively licensed. Acceptable: Public Domain, CC0, CC-BY, CC-BY-SA, MIT, Apache-2.0, BSD-2,
   BSD-3, Unlicense. Also acceptable: public-domain composition (composer died >95 years ago)
   whose engraving is offered freely by the uploader with no contrary terms — see "License
   evidence" below.

   Reject: CC-BY-NC, CC-BY-ND, "free for personal use," "all rights reserved," no license stated,
   proprietary EULAs, anything you're unsure about.

3. A real score: ≥16 measures with notes in ≥1 part. Shorter stubs are acceptable only if they
   exercise a specific coverage-gap feature (below).

4. Uncompressed `.xml` or `.musicxml`. Reject `.mxl` files — do not download and unzip them.

## Coverage gaps (supervisor-provided)

Bias toward files exercising features in this list. Elements marked with * are
completely absent from our corpus (highest priority). Others appear only 1-10 times.

**Highest priority — zero coverage (pick one category and target it):**

- Percussion notation: `percussion`, `beater`, `stick`, `stick-type`, `stick-material`,
  `stick-location`, `timpani`, `metal`, `membrane`, `wood`, `glass`, `pitched`,
  `semi-pitched`, `other-percussion` — a drum score or orchestral percussion part
- Metronome marks with complex notation: `metronome-note`, `metronome-beam`,
  `metronome-dot`, `metronome-tuplet`, `metronome-relation`, `metronome-type`
- Guitar/rhythm slash notation: `beat-repeat`, `slash-dot`, `slash-type`, `barre`
- Ornament/expression detail: `notehead-text`, `other-notation`, `other-play`, `play`,
  `mute`, `string-mute`, `principal-voice`
- Layout elements: `measure-layout`, `measure-distance`, `bookmark`, `link`,
  `credit-image`, `opus`

**Medium priority — present but rare (< 10 occurrences):**

- Guitar chord diagrams: `frame`, `frame-frets`, `frame-strings`, `first-fret`
- Bend technique: `bend`, `bend-alter`, `pre-bend`, `release`
- Accordion: `accordion-registration`, `accordion-high`, `accordion-low`,
  `accordion-middle`
- String harmonics: `harmonic`, `artificial`, `base-pitch`, `sounding-pitch`,
  `touching-pitch`
- Harp pedal diagrams: `harp-pedals`
- Scordatura: `scordatura`

Pick a category that your chosen file exercises. You don't need to find a file that
covers all gaps — one well-exercised category is enough.

## Files already in the corpus

```json
{{found-files}}
```

Skip silently if the candidate's source URL matches, or if filename + domain both match. If the
filename matches but the domain differs, still send a CANDIDATE — it may be a different cut.

## Where to look

Prefer sources with explicit per-file license metadata (Mutopia, MuseScore CC-tagged scores,
Wikimedia Commons, GitHub repos with permissive `LICENSE`). Avoid paywalled, login-gated, or
scrape-forbidden sites (Sheet Music Plus, Musicnotes), and neutral file hosts (Dropbox, Drive)
where no license is stated.

## Procedure

1. Search. Pick a candidate.

2. Download, parse, and fill out the CANDIDATE block. If the file does not parse as
   `<score-partwise>` / `<score-timewise>` with `version="3.0"`, discard and try again.

3. Send the CANDIDATE block to the supervisor and wait:

   ```
   CANDIDATE
   filename: <basename to save as>
   source_url: <direct URL to the file>
   page_url: <hosting page URL, if different>
   license: <SPDX-ish, e.g. CC-BY-4.0, CC0-1.0, MIT, Public-Domain>
   license_evidence: <quoted text + URL where it appears>
   composer: <name>
   title: <work title>
   why_interesting: <one sentence tying to a coverage gap>
   size_bytes: <int>
   measures: <approx int>
   parts: <int>
   ```

4. On `APPROVED`: save the file to `./found-files/<filename>` and write
   `./found-files/<filename>.license.md` using the template below.
   On `REJECTED: <reason>`: discard and return to step 1.

5. Stop. One file per invocation. Do not modify `found-files.json`.

## License evidence

Before sending CANDIDATE, you must be able to quote the license text and link the URL it appears
on. Specifics:

- MuseScore: quote the CC badge text from the score page; link the score page.
- IMSLP: the quoted text must cover the typesetting/edition, not only the composition.
- GitHub: link the `LICENSE` file; confirm the README does not carve out data files.
- Wikimedia Commons: quote the "Licensing" section.

A public-domain composer alone is not sufficient — the engraving needs its own permissive offer.
When in doubt, skip.

## `.license.md` template

```markdown
# <title> — <composer>

- License: <SPDX-ish>
- Source URL: <direct file URL>
- Page URL: <hosting page URL>
- Downloaded: <YYYY-MM-DD UTC>

## License statement (quoted)

> <verbatim quote from source page>

## Why interesting

<one sentence: coverage gap or features exercised>
```

## Stop conditions

Report `NO_CANDIDATE` if:

- 5+ sources tried, every candidate duplicate / wrong version / badly licensed.
- Sites require login, captcha, or payment.
- No candidate has a determinable MusicXML version.

```
NO_CANDIDATE
tried:
  - <url>: <reason>
  - <url>: <reason>
```
