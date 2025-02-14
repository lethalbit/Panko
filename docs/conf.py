# SPDX-License-Identifier: BSD-3-Clause

from pathlib    import Path
from datetime   import date
from subprocess import check_output

PANKO_ROOT  = Path(__file__).resolve().parent.parent
PANKO_SRC   = PANKO_ROOT / 'src'
PANKO_DOCS  = PANKO_ROOT / 'docs'
PANKO_BUILD = PANKO_ROOT / 'build'

PANKO_DOXY_OUT = PANKO_BUILD / 'doxyoutput'
PANKO_DOXY_API = PANKO_DOCS  / 'api'

def get_version() -> str:
	try:
		return check_output(
			('git', 'describe', '--tag', '--always', '--dirty'),
			cwd = PANKO_ROOT
		).decode()
	except Exception:
		return ''

panko_version = get_version()

project   = 'Panko'
version   = panko_version
revision  = version
copyright = f'{date.today().year}, Panko Contributors'
language  = 'en'

extensions = [
	'sphinx.ext.githubpages',
	'sphinx.ext.graphviz',
	'sphinx.ext.napoleon',
	'sphinx.ext.todo',
	'sphinxcontrib.mermaid',
	'myst_parser',
	'sphinx_inline_tabs',
	'sphinxext.opengraph',
	'breathe',
]

source_suffix = {
	'.rst': 'restructuredtext',
	'.md': 'markdown',
}

exclude_patterns = [
	'meson.build',
	'config.py.in',
	'config.py',
	'Doxyfile.in',
	'Doxyfile'
]

pygments_style         = 'monokai'
graphviz_output_format = 'svg'
todo_include_todos     = True

napoleon_google_docstring              = True
napoleon_numpy_docstring               = True
napoleon_use_ivar                      = True
napoleon_use_admonition_for_notes      = True
napoleon_use_admonition_for_examples   = True
napoleon_use_admonition_for_references = True

myst_heading_anchors = 3

html_baseurl     = 'https://lethalbit.github.io/Panko'
html_theme       = 'furo'
html_copy_source = False

html_theme_options = {
	'top_of_page_buttons': [],
}

ogp_site_url = html_baseurl
ogp_image    = f'{html_baseurl}/_images/og-image.png'

linkcheck_anchors_ignore_for_url = [

]

linkcheck_ignore = [

]

primary_domain     = 'cpp'
highlight_language = 'cpp'

cpp_paren_attributes = [

]

cpp_id_attributes = [

]

breathe_projects = {
	project: f'{PANKO_DOXY_OUT}/xml',
}
breathe_default_project = project
